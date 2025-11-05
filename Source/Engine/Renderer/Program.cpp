#include "Program.h"

namespace neu
{
	Program::Program()
	{
		m_program = glCreateProgram();
	}

	Program::~Program()
	{
		if (m_program) glDeleteProgram(m_program);
	}

	bool Program::Load(const std::string& filename) 
	{
		// load program document
		serial::document_t document;
		if (!serial::Load(filename, document)) {
			LOG_WARNING("Could not load program file: {}", filename);
			return false;
		}

		if (!m_program) m_program = glCreateProgram();
		// get/add vertex shader
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

		// get/add fragment shader
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

	void Program::AttachShader(std::shared_ptr<Shader> shader)
	{
		if (!shader || shader->m_shader == 0) {
			LOG_ERROR("AttachShader failed: shader is null or not compiled");
			return;
		}
		glAttachShader(m_program, shader->m_shader);
	}

	bool Program::Link()
	{
		glLinkProgram(m_program);
		GLint success{};
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar info[1024];
			GLsizei len{};
			glGetProgramInfoLog(m_program, sizeof(info), &len, info);
			LOG_ERROR("Program link failed: {}", std::string(info, len));
		}
		return success;
	}

	void Program::Use()
	{
		glUseProgram(m_program);
	}

	void Program::SetUniform(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform1f(location, value);
	}

	void Program::SetUniform(const std::string& name, int value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform1i(location, value);
	}

	void Program::SetUniform(const std::string& name, unsigned int value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform1ui(location, value);
	}

	void Program::SetUniform(const std::string& name, bool value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform1ui(location, value);
	}

	void Program::SetUniform(const std::string& name, const glm::vec2& value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform2f(location, value.x, value.y);
	}

	void Program::SetUniform(const std::string& name, const glm::vec3& value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform3f(location, value.x, value.y, value.z);
	}
	
	void Program::SetUniform(const std::string& name, const glm::mat3& value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Program::SetUniform(const std::string& name, const glm::mat4& value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	
	GLint Program::GetUniformLocation(const std::string& name)
	{
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