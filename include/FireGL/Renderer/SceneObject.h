#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Renderer/Scene.h>
#include <FireGL/Renderer/Transform.h>
#include <FireGL/Renderer/Mesh.h>

namespace fgl
{
	class Material;

	/**
	 * Base class for all game objects that can be stored and managed within the scene container.
	 * Each scene object represents a single predefined shape, model, or entity, with a position and other properties in the world.
	 */
	class SceneObject
	{
	public:
		/**
		 * Constructs a SceneObject and initializes its Transform component.
		 * This constructor is automatically called for classes like Entity, Shape, and Model internally.
		 */		
		SceneObject();

		/** Virtual destructor for proper cleanup of derived classes */
		virtual ~SceneObject() = default;

		/**
		 * Renders the object.
		 * Must be implemented by derived classes to define rendering logic.
		 *
		 * @param NumberInstance Number of instances to render (for instanced rendering).
		 */
		virtual void Render(size_t NumberInstance) const = 0;

		/**
		 * Called during object initialization, when added to the scene.
		 * Used for setup and preparing the object for gameplay.
		 */
		virtual void BeginPlay() = 0;

		/**
		 * Called when the object is removed or destroyed from the scene.
		 * Handles cleanup and deinitialization.
		 */
		virtual void Destroy() = 0;

		/**
		 * Per-frame update method for the object.
		 * Handles logic such as animations or physics that need to update regularly.
		 *
		 * @param DeltaTime Time elapsed since the previous frame.
		 */
		virtual void Tick(float DeltaTime) = 0;

		/**
		 * Sets the material used for rendering the object.
		 *
		 * @param Material Shared pointer to the material to apply.
		 */
		virtual void SetMaterial(std::shared_ptr<Material> Material) = 0;

		/**
		 * Retrieves the material of the object.
		 *
		 * For models with multiple meshes, this function takes an optional `MeshIndex` parameter
		 * to specify which mesh's material to retrieve. For shapes or single-mesh objects,
		 * the index is ignored, and the material of the single mesh is returned.
		 *
		 * @param MeshIndex (Optional) The index of the mesh whose material to retrieve. Defaults to 0.
		 * @return A shared pointer to the Material. Returns `nullptr` if the index is out of bounds
		 *         or the object lacks mesh data.
		 */
		virtual const std::shared_ptr<Material> GetMaterial(size_t MeshIndex = 0) const = 0;

		/**
		 * Assigns this object to a specific scene.
		 * Only the Scene itself should call this method to ensure proper ownership.
		 *
		 * @param Scene Pointer to the owning Scene.
		 */
		void SetScene(Scene* Scene);
	
		/**
		 * Retrieves the scene that owns this object.
		 * This function allows derived classes or components to interact with the scene that the object is part of.
		 *
		 * @return A pointer to the Scene object that owns this SceneObject.
		 */
		const Scene* const GetScene() const;
	
		/**
		 * Accesses the Transform component of this object.
		 * Provides position, rotation, and scale data for spatial calculations.
		 *
		 * @return Reference to the Transform component.
		 */
		Transform& GetTransform();
		const Transform& GetTransform() const;


		/**
		 * Indicates whether this object is newly created.
		 * Used internally by the renderer to perform initialization passes before the object’s first frame.
		 *
		 * @return True if the object is new, otherwise false.
		 */		
		bool IsNew() const;

		/**
		 * Marks the object as new or no longer new.
		 * Only the renderer should call this during initialization.
		 *
		 * @param IsNew Flag to mark the object as new.
		 */
		void SetNew(bool IsNew);

		/**
		 * Retrieves the hash value of this object.
		 * Used for batching objects together in the rendering pipeline for instanced rendering.
		 *
		 * @return Hash value representing the object.
		 */		
		virtual size_t GetHash() const = 0;

		/**
		 * Indicates if this object is a skybox.
		 * Skyboxes are rendered last to avoid depth conflicts and do not use instanced rendering.
		 * This method should only return true in classes derived from Skybox to signal the rendering pipeline
		 * to handle it separately from other objects.
		 *
		 * @return True if the object is a skybox, otherwise false.
		 */		
		virtual bool IsSkybox() const { return false; }

		/**
		 * Retrieves all meshes associated with this object.
		 * This method is called during initialization to ensure the renderer processes each mesh correctly.
		 * Shapes return a single mesh, while models may return multiple meshes.
		 *
		 * @return Vector containing all meshes for the object.
		 */
		virtual std::vector<BaseMesh>& GetMeshes() = 0;

	private:
		/** Pointer to the Scene that owns this object */
		Scene* m_OwningScene;

		/**
		 * The Transform of this object, representing its position, rotation, and scale in the world.
		 * Every SceneObject is initialized with a Transform to ensure proper spatial representation.
		 */
		Transform m_Transform;

		/**
		 * Flag indicating if the object is newly created.
		 * Used internally by the renderer to handle initial rendering passes.
		 */
		bool m_New;
	};

} // namespace fgl