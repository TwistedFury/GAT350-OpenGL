#include "Program.h"
#include <SDL3/SDL.h>

namespace neu {
    namespace {
        inline bool IsContextCurrentAndValid()
        {
            void* ctx = SDL_GL_GetCurrentContext();
            if (ctx == nullptr) {
                LOG_ERROR("OpenGL error: no current context on this thread");
                return false;
            }
            return true;
        }

        inline bool IsProgramValid(GLuint prog)
        {
            if (prog == 0) {
                LOG_ERROR("OpenGL error: program handle is 0");
                return false;
            }
            GLboolean isProgram = glIsProgram(prog);
            if (isProgram == GL_FALSE) {
                LOG_ERROR("OpenGL error: handle {} is not a valid program in current context", prog);
                return false;
            }
            return true;
        }
    }

    Program::Program() {
        m_program = 0;
    }

    Program::~Program()
    {
        // Only delete if a GL context is current; avoid driver faults during shutdown.
        if (SDL_GL_GetCurrentContext() != nullptr && m_program != 0) {
            GLboolean isProgram = glIsProgram(m_program);
            if (isProgram == GL_TRUE) {
                glDeleteProgram(m_program);
            }
        }
        m_program = 0;
    }

    bool Program::Load(const std::string& filename) {
        if (!IsContextCurrentAndValid()) return false;

        void* ctx = SDL_GL_GetCurrentContext();
        LOG_INFO("Program::Load ctx={} prog={}", ctx, m_program);

        serial::document_t document;
        if (!serial::Load(filename, document)) {
            LOG_WARNING("Could not load program file: {}", filename);
            return false;
        }

        if (m_program == 0) {
            m_program = glCreateProgram();
            if (m_program == 0) {
                LOG_ERROR("Program::Load failed: glCreateProgram returned 0");
                return false;
            }
        }
        LOG_INFO("Program state after create: prog={} ctx={}", m_program, ctx);

        std::string shaderName;

        SERIAL_READ_NAME(document, "vertex_shader", shaderName);
        if (!shaderName.empty()) {
            auto shader = neu::Resources().Get<neu::Shader>(shaderName, GL_VERTEX_SHADER);
            if (!shader) {
                LOG_WARNING("Could not get vertex shader: {}", shaderName);
                glDeleteProgram(m_program);
                m_program = 0;
                return false;
            }
            AttachShader(shader);
        }

        SERIAL_READ_NAME(document, "fragment_shader", shaderName);
        if (!shaderName.empty()) {
            auto shader = neu::Resources().Get<neu::Shader>(shaderName, GL_FRAGMENT_SHADER);
            if (!shader) {
                LOG_WARNING("Could not get fragment shader: {}", shaderName);
                glDeleteProgram(m_program);
                m_program = 0;
                return false;
            }
            AttachShader(shader);
        }

        return Link();
    }

    void Program::AttachShader(std::shared_ptr<Shader> shader) {
        if (!IsContextCurrentAndValid()) return;

        void* ctx = SDL_GL_GetCurrentContext();
        LOG_INFO("Program::AttachShader ctx={} prog={} shader={}", ctx, m_program, shader ? shader->m_shader : 0u);

        if (!shader || shader->m_shader == 0) {
            LOG_ERROR("AttachShader failed: shader null/not compiled");
            return;
        }
        if (m_program == 0) {
            m_program = glCreateProgram();
            if (m_program == 0) {
                LOG_ERROR("AttachShader failed: glCreateProgram returned 0");
                return;
            }
        }
        if (!IsProgramValid(m_program)) return;

        glAttachShader(m_program, shader->m_shader);
    }

    bool Program::Link() {
        if (!IsContextCurrentAndValid()) return false;
        if (!IsProgramValid(m_program)) return false;

        void* ctx = SDL_GL_GetCurrentContext();
        LOG_INFO("Program::Link ctx={} prog={}", ctx, m_program);

        GLint attached = 0;
        glGetProgramiv(m_program, GL_ATTACHED_SHADERS, &attached);
        if (attached == 0) {
            LOG_ERROR("Program::Link failed: no shaders attached to program {}", m_program);
            return false;
        }

        glLinkProgram(m_program);

        GLint success = GL_FALSE;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            GLchar info[1024];
            GLsizei len = 0;
            glGetProgramInfoLog(m_program, sizeof(info), &len, info);
            LOG_ERROR("Program link failed: {}", std::string(info, len));
            return false;
        }

        // Invalidate cached uniform locations after relink
        m_uniformLocations.clear();
        return true;
    }

    void Program::Use() {
        if (!IsContextCurrentAndValid()) return;
        if (!IsProgramValid(m_program)) return;
        glUseProgram(m_program);
    }

    void Program::SetUniform(const std::string& name, float value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) glUniform1f(location, value);
    }

    void Program::SetUniform(const std::string& name, int value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) glUniform1i(location, value);
    }

    void Program::SetUniform(const std::string& name, unsigned int value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) glUniform1ui(location, value);
    }

    void Program::SetUniform(const std::string& name, bool value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) glUniform1i(location, value ? 1 : 0);
    }

    void Program::SetUniform(const std::string& name, const glm::vec2& value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) glUniform2f(location, value.x, value.y);
    }

    void Program::SetUniform(const std::string& name, const glm::vec3& value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) glUniform3f(location, value.x, value.y, value.z);
    }
    
    void Program::SetUniform(const std::string& name, const glm::mat3& value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void Program::SetUniform(const std::string& name, const glm::mat4& value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    
    GLint Program::GetUniformLocation(const std::string& name) {
        if (SDL_GL_GetCurrentContext() == nullptr) {
            LOG_ERROR("GetUniformLocation failed: no current GL context");
            return -1;
        }
        if (m_program == 0 || glIsProgram(m_program) == GL_FALSE) {
            LOG_ERROR("GetUniformLocation failed: invalid program {}", m_program);
            return -1;
        }

        auto it = m_uniformLocations.find(name);
        if (it != m_uniformLocations.end()) return it->second;

        GLint location = glGetUniformLocation(m_program, name.c_str());
        if (location == -1) {
            LOG_WARNING("Uniform not found: {}", name);
        }
        m_uniformLocations[name] = location;
        return location;
    }
}