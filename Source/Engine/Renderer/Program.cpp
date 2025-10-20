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

	void Program::AttachShader(std::shared_ptr<Shader> shader)
	{
		glAttachShader(m_program, shader->m_shader);
	}

	void Program::Link()
	{
		glLinkProgram(m_program);
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

	void Program::SetUniform(const std::string& name, uint16_t value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform1ui(location, value);
	}

	void Program::SetUniform(const std::string& name, bool value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform1ui(location, value);
	}

	void Program::SetUniform(const std::string& name, const vec2& value)
	{
		GLint location = GetUniformLocation(name);
		if (location != -1) glUniform2f(location, value.x, value.y);
	}

	void Program::SetUniform(const std::string& name, const vec3& value)
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