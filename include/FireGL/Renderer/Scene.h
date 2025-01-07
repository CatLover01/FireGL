#pragma once

#include <FireGL/fglpch.h>

namespace fgl
{
	class BaseCamera;
	class SceneObject;

	/**
	 * Scene Class
	 *
	 * Overview:
	 * This class represents a 3D scene containing objects and cameras. It manages the addition and processing
	 * of scene objects and handles active camera logic. The class provides basic scene management utilities,
	 * but can be extended to include additional functionality like object deletion based on tags or handling
	 * multiple cameras.
	 *
	 * Key Notes:
	 * - Designed to store and manage scene objects exclusively through unique pointers (RAII compliant).
	 * - For correct ownership transfer, objects should be passed to the scene using std::move().
	 * - This class can manage multiple cameras and switch between them.
	 * - The renderer accesses scene objects directly through the provided GetObjects() method.
	 */
	class Scene
	{
	public:
		/**
		 * Default constructor for the Scene class.
		 *
		 * @note Make sure to set the active camera with SetActiveCamera() before processing the scene.
		 */
		Scene();

		/**
		 * Constructs a Scene with a specified active camera.
		 *
		 * @param ActiveCamera A shared pointer to the initial active camera for the scene.
		 */
		Scene(std::shared_ptr<BaseCamera> ActiveCamera);

		/** Virtual destructor for proper cleanup of derived classes */
		virtual ~Scene() = default;

		/**
		 * Adds a new object to the scene, transferring ownership.
		 * The passed object is set to nullptr after the transfer.
		 *
		 * @param Object A unique pointer to the SceneObject being added.
		 */		
		void AddObject(std::unique_ptr<SceneObject> Object);
		
		/**
		 * Processes all objects in the scene and updates the active camera.
		 *
		 * @note This function relies on TimeManager to update each object.
		 */
		void Process();

		/**
		 * Retrieves the list of all objects in the scene.
		 * This function is called by the renderer to access and render scene objects.
		 *
		 * @return A const reference to the vector of unique pointers to SceneObjects.
		 */		
		const std::vector<std::unique_ptr<SceneObject>>& GetObjects() const;

		/**
		 * Sets the active camera for the scene.
		 *
		 * @param ActiveCamera A shared pointer to the camera to be set as active.
		 */
		void SetActiveCamera(std::shared_ptr<BaseCamera> ActiveCamera);

		/**
		 * Retrieves the currently active camera.
		 *
		 * @return A shared pointer to the active camera.
		 */		
		std::shared_ptr<BaseCamera> GetActiveCamera() const;

		/**
		 * Adds a new camera to the scene's camera list.
		 *
		 * @param Camera A shared pointer to the camera being added.
		 */
		void AddCamera(std::shared_ptr<BaseCamera> Camera);

		/**
		 * Removes a specified camera from the scene.
		 *
		 * @param Camera A shared pointer to the camera being removed.
		 */
		void RemoveCamera(std::shared_ptr<BaseCamera> Camera);

	protected:
		/** A collection of unique pointers to the objects within the scene. */
		std::vector<std::unique_ptr<SceneObject>> m_Objects;

		/** A shared pointer to the currently active camera. */
		std::shared_ptr<BaseCamera> m_ActiveCamera;

		/** A collection of shared pointers to all cameras in the scene. */
		std::vector<std::shared_ptr<BaseCamera>> m_Cameras;
	};

} // namespace fgl