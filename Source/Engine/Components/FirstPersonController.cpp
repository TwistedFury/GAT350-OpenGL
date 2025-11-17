#include "FirstPersonController.h"
#include "Engine.h"

namespace neu {
	FACTORY_REGISTER(FirstPersonController)

	bool FirstPersonController::Start()
	{
		glm::vec3 euler = owner->transform.GetRotation();
		pitch = euler.y;
		yaw = euler.x;

		return true;
	}

	void FirstPersonController::Update(float dt) {
		// get camera rotation
		if (GetEngine().GetInput().GetMouseButtonDown(InputSystem::MouseButton::Right))
		{
			glm::vec2 axis = GetEngine().GetInput().GetMouseDelta() * sensitivity;

			yaw -= axis.x;
			pitch -= axis.y;

			// Clamp pitch to prevent flipping
			pitch = glm::clamp(pitch, -89.0f, 89.0f);
		}

		// Manual quaternion construction - pitch around X, yaw around Y
		glm::quat qPitch = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
		glm::quat q = qYaw * qPitch;  // Apply yaw first, then pitch
		owner->transform.SetRotationQuat(q);

		glm::vec3 direction{ 0 };
		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction -= owner->transform.Right();
		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction += owner->transform.Right();
		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction += owner->transform.Up();
		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction -= owner->transform.Up();
		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction += owner->transform.Forward();
		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction -= owner->transform.Forward();;

		if (glm::length2(direction) > 0.0f) {
			direction = glm::normalize(direction);
		}
		owner->transform.position += direction * (speed * dt);
	}

	void FirstPersonController::Read(const serial_data_t& value) {
		SERIAL_READ(value, speed);
		SERIAL_READ(value, sensitivity);
	}

	void FirstPersonController::UpdateGui() {
		ImGui::DragFloat("Speed", &speed, 0.1f);
		ImGui::DragFloat("Sensitivity", &sensitivity, 0.1f);

		// Show actual camera orientation
		ImGui::Separator();
		ImGui::Text("Camera Orientation:");
		ImGui::Text("  Pitch: %.1f", pitch);
		ImGui::Text("  Yaw: %.1f", yaw);
	}
}