#include "CameraComponent.h"
#include <Engine.h>

namespace neu
{
	FACTORY_REGISTER(CameraComponent);

	void neu::CameraComponent::Update(float dt)
	{
		view = glm::lookAt(owner->transform.position, owner->transform.position + owner->transform.Forward(), owner->transform.Up());
		projection = glm::perspective(glm::radians(fov), aspect, near, far);
	}

	void neu::CameraComponent::SetPerspective(float fov, float aspect, float near, float far)
	{
		this->fov = fov; this->aspect = aspect; this->near = near; this->far = far;
		//projection = glm::perspective(glm::radians(fov), aspect, near, far);
	}

	void neu::CameraComponent::SetLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
	{
		view = glm::lookAt(eye, center, up);
	}

	void neu::CameraComponent::SetProgram(Program& program)
	{
		program.SetUniform("u_view", view);
		program.SetUniform("u_projection", projection);
	}

	void neu::CameraComponent::Read(const serial_data_t& value)
	{
		SERIAL_READ(value, fov);
		if (!SERIAL_READ(value, aspect)) aspect = GetEngine().GetRenderer().GetWidth() / (float)GetEngine().GetRenderer().GetHeight();
		SERIAL_READ(value, near);
		SERIAL_READ(value, far);
	}

	void neu::CameraComponent::UpdateGui()
	{
		ImGui::DragFloat("FOV", &fov, 0.1f);
		ImGui::DragFloat("Aspect", &aspect, 0.1f);
		ImGui::DragFloat("Near", &near, 0.1f);
		ImGui::DragFloat("Far", &far, 0.1f);
	}
}