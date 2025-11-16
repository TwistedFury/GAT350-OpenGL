#include "Program.h"
#include <SDL3/SDL.h>

namespace neu {
    Program::Program() {
        // Do not touch GL until a context is current
        m_program = 0;
    }

    Program::~Program()
    {
        glDeleteProgram(m_program);
    }

    bool Program::Load(const std::string& filename) {
        void* ctx = SDL_GL_GetCurrentContext();
        LOG_INFO("Program::Load ctx={} prog={}", ctx, m_program);
        if (ctx == nullptr) {
            LOG_ERROR("Program::Load failed: no current OpenGL context");
            return false;
        }

        serial::document_t document;
        if (!serial::Load(filename, document)) {
            LOG_WARNING("Could not load program file: {}", filename);
            return false;
        }

        if (m_program == 0) m_program = glCreateProgram();
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
        void* ctx = SDL_GL_GetCurrentContext();
        LOG_INFO("Program::AttachShader ctx={} prog={} shader={}", ctx, m_program, shader ? shader->m_shader : 0u);

        if (!shader || shader->m_shader == 0) {
            LOG_ERROR("AttachShader failed: shader null/not compiled");
            return;
        }
        if (ctx == nullptr) {
            LOG_ERROR("AttachShader failed: no current OpenGL context");
            return;
        }
        if (!m_program) {
            m_program = glCreateProgram();
            if (!m_program) {
                LOG_ERROR("AttachShader failed: program not created");
                return;
            }
        }
        glAttachShader(m_program, shader->m_shader);
    }

    bool Program::Link() {
        void* ctx = SDL_GL_GetCurrentContext();
        LOG_INFO("Program::Link ctx={} prog={}", ctx, m_program);
        if (ctx == nullptr) {
            LOG_ERROR("Program::Link failed: no current OpenGL context");
            return false;
        }

        glLinkProgram(m_program);
        GLint success{};
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar info[1024];
            GLsizei len{};
            glGetProgramInfoLog(m_program, sizeof(info), &len, info);
            LOG_ERROR("Program link failed: {}", std::string(info, len));
        }
        return success != 0;
    }

    void Program::Use() {
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
        if (location != -1) glUniform1ui(location, value);
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
        auto it = m_uniformLocations.find(name);
        if (it == m_uniformLocations.end())
        {
            GLint location = glGetUniformLocation(m_program, name.c_str());
            if (location == -1)
            {
                LOG_WARNING("Could not find uniform: {}", name);
            }
            m_uniformLocations[name] = location;
        }
        return m_uniformLocations[name];
    }
}