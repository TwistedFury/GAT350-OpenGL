#pragma once

#include "Framework/Component.h"
#include "Renderer/Program.h"
#include "Renderer/RenderTexture.h"

namespace neu 
{
	class CameraComponent : public Component
	{
	public:
		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};

		CLASS_PROTOTYPE(CameraComponent)

		void Update(float dt) override;

		void SetPerspective(float fov, float aspect, float near, float far);
		void SetLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up = { 0, 1, 0 });

		void SetProgram(Program& program);

		void Read(const serial_data_t& value) override;
		void UpdateGui() override;

		void Clear();

	public:
		glm::mat4 projection{ 1 };
		glm::mat4 view{ 1 };

		ProjectionType projectionType = ProjectionType::Perspective;
		bool shadowCamera{ false };

		float fov = 70.0f;
		float aspect = 0;
		float near = 0.1f;
		float far = 100.0f;
		float size = 5.0f;

		res_t<RenderTexture> outputTexture;
		bool clearColorBuffer{ true };
		bool clearDepthBuffer{ true };
		glm::vec3 backgroundColor{ 0, 0, 0 };
	};
}