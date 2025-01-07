#include <FireGL/Renderer/BaseCamera.h>

#include <External/glm/gtc/matrix_transform.hpp>

namespace fgl {

	BaseCamera::BaseCamera()
		: m_View(glm::mat4(0.f)),
		m_CameraTransform(nullptr),
		m_Up(glm::vec3(0.f, 1.f, 0.f)),
		m_WorldUp(glm::vec3(0.f, 1.f, 0.f))
	{
		UpdateCameraVectors();
	}

	BaseCamera::BaseCamera(glm::vec3 CameraPosition, glm::vec3 CameraUpVector)
		: m_CameraTransform(nullptr), m_Up(CameraUpVector), m_WorldUp(CameraUpVector)
	{
		m_CameraTransform.SetPosition(CameraPosition);
		UpdateCameraVectors();
	}

	void BaseCamera::SetPerspective(float FOVAngle, float WindowRatio, float Near, float Far)
	{
		m_Projection = glm::perspective(glm::radians(FOVAngle), WindowRatio, Near, Far);
	}

	void BaseCamera::SetOrthographic(float Left, float Right, float Top, float Bottom)
	{
		m_Projection = glm::ortho(Left, Right, Top, Bottom);
	}

	void BaseCamera::UpdateRotationInput(double XPos, double YPos)
	{
		float XPosOut = static_cast<float>(XPos);
		float YPosOut = static_cast<float>(YPos);

		if (m_FirstInput)
		{
			m_InputCoords.x = XPosOut;
			m_InputCoords.y = YPosOut;
			m_FirstInput = false;
		}

		float XOffset = XPosOut - m_InputCoords.x;
		// Invert YOffset due to Y-axis inversion in screen space
		float YOffset = m_InputCoords.y - YPosOut;
		m_InputCoords.x = XPosOut;
		m_InputCoords.y = YPosOut;

		// Only process the mouse movement if there is any offset
		if (XOffset != 0.0f || YOffset != 0.0f)
		{
			ProcessRotationInput(XOffset, YOffset);

			// Update the camera's vectors as the rotation of the camera should be changed
			UpdateCameraVectors();
		}
	}

	void BaseCamera::UpdateViewMatrix()
	{
		m_View = glm::lookAt(m_CameraTransform.GetPosition(), m_CameraTransform.GetPosition() + m_Front, m_Up);
	}

	glm::mat4 BaseCamera::GetViewMatrix() const
	{
		return m_View;
	}

	glm::mat4 BaseCamera::GetProjectionMatrix() const
	{
		return m_Projection;
	}

	glm::vec3 BaseCamera::GetFrontVector() const
	{
		return m_Front;
	}

	glm::vec3 BaseCamera::GetRightVector() const
	{
		return m_Right;
	}

	glm::vec3 BaseCamera::GetUpVector() const
	{
		return m_Up;
	}

	Transform& BaseCamera::GetCameraTransform() {
		return m_CameraTransform;
	}

	void BaseCamera::ComputeModelViewProjection(glm::mat4& OutModelViewProjection, glm::mat4& OutModelMatrix) const
	{
		glm::mat4 ModelMatrix =
			glm::translate(glm::mat4(1.0f), m_CameraTransform.GetPosition()) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraTransform.GetRotation().x), glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraTransform.GetRotation().y), glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraTransform.GetRotation().z), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), m_CameraTransform.GetScale());
		
		OutModelViewProjection = GetProjectionMatrix() * GetViewMatrix() * ModelMatrix;
		OutModelMatrix = ModelMatrix;
	}

	void BaseCamera::UpdateCameraVectors()
	{
		glm::vec3 direction = glm::vec3();
		direction.x = cos(glm::radians(m_CameraTransform.GetRotation().x)) * cos(glm::radians(m_CameraTransform.GetRotation().y));
		direction.y = sin(glm::radians(m_CameraTransform.GetRotation().y));
		direction.z = sin(glm::radians(m_CameraTransform.GetRotation().x)) * cos(glm::radians(m_CameraTransform.GetRotation().y));
		m_Front = glm::normalize(direction); // Camera Direction Vector (Front)

		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}

} // namespace fgl