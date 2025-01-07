#pragma once

#include <FireGL/Renderer/Transform.h>

#include <External/glm/glm.hpp>

namespace fgl 
{

	enum class CameraMovement
	{
		Forward,      ///< Move the camera forward along its front vector.
		Backward,     ///< Move the camera backward along its front vector.
		Rightwards,   ///< Move the camera to the right, relative to its current orientation.
		Leftwards,    ///< Move the camera to the left, relative to its current orientation.
		Upwards,      ///< Move the camera upwards, along the world's up vector.
		Downwards     ///< Move the camera downwards, along the world's up vector.
	};


	/**
	 * The BaseCamera class represents a camera in 3D space, providing basic functionality
	 * for controlling the camera’s position, orientation, and projection. It supports both
	 * perspective and orthographic views, as well as user input handling coming from the InputManager.
	 *
	 * This class serves as a foundation for building more complex cameras and is intended
	 * to be easily extended and used in a 3D graphics pipeline.
	 *
	 * The camera's transform, view, and projection matrices are updated each frame(if needed) to reflect
	 * changes in the camera's position and orientation, ensuring that objects in the scene are
	 * rendered correctly from the camera’s point of view.
	 */
	class BaseCamera
	{
	public:
		/**
		 * Initializes the camera with a default position and world-up vector.
		 */
		BaseCamera();
		
		/**
		 * Constructor to initialize the camera with a specific position and an optional up vector.
		 * This constructor allows for custom camera positioning and orientation.
		 *
		 * @param CameraPosition The initial position of the camera in world space.
		 * @param CameraUpVector The up vector defining the camera's vertical direction (default is (0.f, 1.f, 0.f) in opengl).
		 */
		BaseCamera(glm::vec3 CameraPosition, glm::vec3 CameraUpVector = glm::vec3(0.f, 1.f, 0.f));
		
		/**
		 * Updates the mouse position and calculates the movement offset.
		 * This method should be called by your derived InputManager class
		 * to update the camera's rotation based on mouse movement(see the Example folder).
		 *
		 * @param XPos The current horizontal position of the input(e.g., mouse or joystick).
		 * @param YPos The current vertical position of the input(e.g., mouse or joystick).
		 */
		void UpdateRotationInput(double XPos, double YPos);

		/**
		 * Updates the view matrix of the camera, recalculating the camera's view based on its position
		 * and orientation. This is called once per frame by the scene class to update the camera's perspective.
		 * Calling this function manually has no effect and only wastes compute time.
		 */
		void UpdateViewMatrix();
		
		
		/// Getters
		
		/**
		 * Returns the view matrix of the camera, which defines the camera's position and orientation
		 * relative to the world.
		 *
		 * @return The camera's view matrix.
		 */
		glm::mat4 GetViewMatrix() const;

		/**
		 * Returns the projection matrix of the camera, which defines how the 3D scene is projected onto the 2D screen.
		 * This matrix is updated based on the camera's settings (perspective or orthographic).
		 *
		 * @return The camera's projection matrix.
		 */
		glm::mat4 GetProjectionMatrix() const;
		
		/**
		 * @return the forward direction vector of the camera, indicating where the camera is looking.
		 */
		glm::vec3 GetFrontVector() const;
		
		/**
		 * @return the right direction vector of the camera, used to move the camera left/right in world space.
		 */
		glm::vec3 GetRightVector() const;

		/**
		 * @return the up direction vector of the camera, which determines which direction is "up" for the camera.
		 */
		glm::vec3 GetUpVector() const;

		/**
		 * Returns a reference to the camera's transform, which holds the camera's position, rotation, and scale.
		 * This allows for direct manipulation of the camera’s transform in world space.
		 *
		 * Note: Retrieving the MVP (Model-View-Projection) directly from the transform will not work.
		 * To compute the correct MVP matrix, use the "ComputeModelViewProjection" function provided by this class.
		 *
		 * @return A reference to the camera's transform.
		 */
		Transform& GetCameraTransform();

		/**
		 * Computes the Model-View-Projection (MVP) matrix for the camera.
		 * This function calculates the full MVP matrix required for rendering based on the camera's view matrix,
		 * projection matrix, and the provided model matrix. It should be called whenever the MVP is needed.
		 *
		 * Important: Use this function instead of attempting to compute the MVP through the Transform class directly.
		 * The Transform class does not have the necessary context for computing the view and projection matrices.
		 *
		 * @param OutModelViewProjection Output parameter for the computed MVP matrix.
		 * @param OutModelMatrix Output parameter for the model matrix, which is passed along for convenience if needed.
		 */
		void ComputeModelViewProjection(glm::mat4& OutModelViewProjection, glm::mat4& OutModelMatrix) const;

		/**
		 * Sets the camera's projection mode to perspective, which is useful for simulating 3D depth in a scene.
		 *
		 * @param FOVAngle The field of view angle for the camera.
		 * @param WindowRatio The aspect ratio of the window.
		 * @param Near The near clipping plane distance.
		 * @param Far The far clipping plane distance.
		 */		
		void SetPerspective(float FOVAngle, float WindowRatio, float Near, float Far);

		/**
		 * Sets the camera's projection mode to orthographic, which is useful for 2D rendering or non-perspective views.
		 *
		 * @param Left The left boundary of the orthographic view.
		 * @param Right The right boundary of the orthographic view.
		 * @param Top The top boundary of the orthographic view.
		 * @param Bottom The bottom boundary of the orthographic view.
		 */
		void SetOrthographic(float Left, float Right, float Top, float Bottom);

		/**
		 * Virtual method that must be implemented in derived classes to handle input for camera movement.
		 * The movement direction is specified using the CameraMovement enum, and the delta time (time since the last frame)
		 * is used to update the camera’s position smoothly. Movement input can come from a keyboard, joystick, or controller.
		 *
		 * @param MovementDirection The direction in which to move the camera (forward, backward, left, right, up, down).
		 * @param DeltaTime The time difference between the current and previous frame.
		 */
		virtual void ProcessMovementInput(CameraMovement MovementDirection, float DeltaTime) = 0;

		/**
		 * Virtual method that must be implemented in derived classes to handle rotational movement input.
		 * This is typically used to rotate the camera based on input from devices such as a mouse, joystick, or controller.
		 *
		 * @param XOffset The horizontal movement input (difference from the last frame).
		 * @param YOffset The vertical movement input (difference from the last frame).
		 */
		virtual void ProcessRotationInput(float XOffset, float YOffset) = 0;

	private:
		/**
		 * Updates the camera's front, right, and up vectors based on the current camera rotation.
		 * This method is called internally whenever the camera’s orientation changes.
		 */
		void UpdateCameraVectors();

	private:
		/** Transform represents the camera's position and rotation in 3D space */
		Transform m_CameraTransform;

		/** Vectors for the camera's orientation (front, right, up) */		
		glm::vec3 m_Front, m_Up, m_Right;
		
		/** World up vector, used to calculate the camera's orientation which is (0,1,0) by default in opengl */
		const glm::vec3 m_WorldUp;

		/** Cached view matrix to avoid recalculating multiple time every frame but just once */
		glm::mat4 m_View;
		
		/** Cached projection matrix, updated when the camera's projection settings change */
		glm::mat4 m_Projection;

	private:
		/** Variables for tracking input between frames(e.g., mouse, joystick) */
		bool m_FirstInput = true;   ///< Ensures input coordinates are initialized only once
		glm::vec2 m_InputCoords;	///< Stores the last input coordinates for calculating movement offsets
	};

} // namespace fgl