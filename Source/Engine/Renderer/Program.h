#pragma once
#include "Resources/Resource.h"
#include <glad/glad.h>
//#include <glm/glm.hpp>

namespace neu
{
	class Shader;

	class Program : public Resource
	{
	public:
		Program();
		~Program();

		void AttachShader(std::shared_ptr<Shader> shader);
		void Link();
		void Use();

		// UNIFORM TIME
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, uint16_t value);
		void SetUniform(const std::string& name, bool value);

		void SetUniform(const std::string& name, const vec2& value);
		void SetUniform(const std::string& name, const vec3& value);

		/*
		void SetUniform(const std::string& name, const glm::mat3& value);
		void SetUniform(const std::string& name, const glm::mat4& value);
		*/

	public:
		GLuint m_program = 0;
		std::map<std::string, GLint> m_uniformLocations;

	private:
		GLint GetUniformLocation(const std::string& name);
	};
}