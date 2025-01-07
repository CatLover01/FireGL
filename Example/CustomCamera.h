#pragma once

#include <FireGL/Renderer/BaseCamera.h>

using namespace fgl;

class CustomCamera : public BaseCamera
{
public:
	CustomCamera()
		: BaseCamera()
	{
	}

        // Process movement input based on direction and time delta (used for camera movement)
	virtual void ProcessMovementInput(CameraMovement MovementDirection, float DeltaTime) override
	{
		 // Calculate velocity based on movement speed and time
		float velocity = CameraMovementSpeed * DeltaTime;
		switch (MovementDirection)
		{
		case CameraMovement::Forward:
			GetCameraTransform().SetPosition(GetCameraTransform().GetPosition() + velocity * GetFrontVector());
			break;
		case CameraMovement::Backward:
			GetCameraTransform().SetPosition(GetCameraTransform().GetPosition() - velocity * GetFrontVector());
			break;
		case CameraMovement::Leftwards:
			GetCameraTransform().SetPosition(GetCameraTransform().GetPosition() - velocity * GetRightVector());
			break;
		case CameraMovement::Rightwards:
			GetCameraTransform().SetPosition(GetCameraTransform().GetPosition() + velocity * GetRightVector());
			break;
		default:
			return;
		}
	}

	// Process rotation input for the camera, modifying rotation based on X and Y offset values
	virtual void ProcessRotationInput(float XOffset, float YOffset) override
	{
		XOffset *= CameraSensitivity;
		YOffset *= CameraSensitivity;

		GetCameraTransform().SetRotation(glm::vec3(GetCameraTransform().GetRotation().x + XOffset, GetCameraTransform().GetRotation().y, GetCameraTransform().GetRotation().z));
		GetCameraTransform().SetRotation(glm::vec3(GetCameraTransform().GetRotation().x, GetCameraTransform().GetRotation().y + YOffset, GetCameraTransform().GetRotation().z));

		// Update pitch (up-down), but clamp it to avoid gimbal lock
		if (GetCameraTransform().GetRotation().y > 89.0f)
			GetCameraTransform().SetRotation(glm::vec3(GetCameraTransform().GetRotation().x, 89.0f, GetCameraTransform().GetRotation().z));
		if (GetCameraTransform().GetRotation().y < -89.0f)
			GetCameraTransform().SetRotation(glm::vec3(GetCameraTransform().GetRotation().x, -89.0f, GetCameraTransform().GetRotation().z));
	}

	// Adjust camera movement speed based on scroll input (zoom in/out)
	void AddMovementSpeed(float yoffset)
	{
		CameraMovementSpeed = glm::clamp(CameraMovementSpeed + yoffset, CameraMinimalMovementSpeed, CameraMaximalMovementSpeed);
	}

private:
	float CameraMovementSpeed = 50.f; ///< Default camera movement speed
	float CameraSensitivity = 0.25f;  ///< Camera rotation sensitivity

	 // Minimum and maximum movement speed limits
	float CameraMinimalMovementSpeed = 0.5f;
	float CameraMaximalMovementSpeed = 150.f;
};
