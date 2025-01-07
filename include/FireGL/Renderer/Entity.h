#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Renderer/SceneObject.h>
#include <FireGL/Renderer/Material.h>
#include <FireGL/Core/BaseLog.h>

namespace fgl
{
	class Component;
	class Mesh;
	class Transform;

	/**
	 * @class Entity
	 * @brief Represents an entity within a scene, associating it with either a model or a shape.
	 *
	 * This class extends SceneObject and is responsible for managing various aspects of the entity,
	 * including its components and transformations. It provides functionality to attach materials,
	 * create and retrieve components, and handle lifecycle events of the entity. This allows for
	 * flexible and dynamic manipulation of scene entities in the rendering pipeline.
	 */
	class Entity : public SceneObject
	{
	public:
		/** 
		 * @brief Template constructor for creating an Entity with a given object.
		 *
		 * This constructor can accept any object that is derived from SceneObject, such as
		 * Model or Shape. It initializes the entity with the provided object and sets the
		 * scene object in the associated material.
		 *
		 * **IMPORTANT:** When creating a class derived from Entity, **YOU MUST** explicitly call
		 * this constructor within the derived class's constructor. Failure to do so will result
		 * in the entity being improperly initialized, which can lead to rendering errors or crashes.
		 *
		 * @tparam T The type of the object, which must be derived from SceneObject.
		 * @param object A shared pointer to the object being used to initialize the entity.
		 */
		template <typename T, std::enable_if_t<std::is_base_of_v<SceneObject, T>, bool> = true> 
		Entity(std::shared_ptr<T> Object);

		/**
		 * Sets the material for the entity's object.
		 *
		 * @note This will override any previously loaded texture associated with the object.
		 * Unexpected visual results may occur if the material is not compatible.
		 *
		 * @param Material A shared pointer to the material to apply to the object.
		 */		
		virtual void SetMaterial(std::shared_ptr<Material> Material) override final;

		/**
		 * Retrieves the material applied to the entity's object.
		 *
		 * @param MeshIndex The index of the mesh to retrieve the material from (default is 0).
		 * @return A shared pointer to the material associated with the specified mesh.
		 */
		virtual const std::shared_ptr<Material> GetMaterial(size_t MeshIndex = 0) const override final;

		/**
		 * Retrieves all meshes associated with the entity.
		 *
		 * @return A reference to a vector containing all meshes attached to the entity.
		 */
		virtual std::vector<BaseMesh>& GetMeshes() override;

		/**
		 * Retrieves the previously generated unique hash for the entity.
		 *
		 * @return A size_t representing the entity's hash value.
		 */
		virtual size_t GetHash() const override;

		/**
		 * Creates and attaches a component of type T to the entity.
		 *
		 * @tparam T The type of component to create (must derive from Component).
		 * @return A pointer to the created component, or nullptr if a component of this type already exists.
		 */
		template<typename T>
		T* CreateComponent();
		
		/**
		 * Retrieves a component of type T attached to the entity.
		 *
		 * @tparam T The type of component to retrieve (must derive from Component).
		 * @return A pointer to the component, or nullptr if no component of this type exists.
		 */
		template<typename T>
		T* GetComponent();

		/**
		 * Removes a component by its unique identifier.
		 *
		 * @param ID The unique ID of the component to remove.
		 */
		void RemoveComponent(size_t ID);

	protected:
		/** Called every frame to update the entity. */
		virtual void OnTick(float DeltaTime);

		/** Called when the entity is first initialized (at the start of play). */
		virtual void OnBeginPlay();

		/** Called when the entity is going to be destroyed. */
		virtual void OnDestroyed();

		/**
		 * Called before the rendering process begins to set up any necessary states or bindings.
		 * This function is often used for operations such as changing the depth function, binding buffers,
		 * or preparing objects (e.g., skybox setup).
		 * Example: You might change the depth function here before rendering and bind any resources.
		 */
		virtual void OnPrepareRender() const;

		/**
		 * Called after the rendering process is complete to clean up or restore any states.
		 * This function is often used to remove bindings or restore the depth function to its original state,
		 * ensuring that the rendering environment is returned to its pre-render state.
		 * Example: After rendering, you might undo depth function changes or unbind any buffers to restore the original setup.
		 */
		virtual void OnPostRender() const;

	private:
		/** Renders the entity's object. */
		virtual void Render(size_t NumberInstance) const override final;

		/** Ticks the entity, updating its state. */
		virtual void Tick(float DeltaTime) override final;

		/** Handles initialization logic at the start of play. */
		virtual void BeginPlay() override final;

		/** Destroys the entity and performs cleanup operations. */
		virtual void Destroy() override	final;

	private:
		std::shared_ptr<SceneObject> m_Object; ///< The object associated with this entity.

		/**
		 * Stores components attached to the entity.
		 * Only one component of each type can be attached at a time.
		 */		
		std::unordered_map<size_t, std::unique_ptr<Component>> m_Components;
	};

	/**
	 * Implementation of Template constructor - Initializing an entity with a given object.
	 */
	template <typename T, std::enable_if_t<std::is_base_of_v<SceneObject, T>, bool>>
	Entity::Entity(std::shared_ptr<T> Object)
		: SceneObject(), m_Object(Object)
	{
		m_Object->GetMaterial()->SetSceneObject(this);
	}

	/**
	 * Implementation of CreateComponent - Attaches a new component to the entity.
	 */
	template<typename T>
	T* Entity::CreateComponent()
	{
		if (!GetComponent<T>())
		{
			LOG_ASSERT((std::is_base_of<Component, T>::value), "Invalid template parameter! Type T must derive from the Component class.")
			size_t ID = typeid(T).hash_code();

			std::unique_ptr<T> ComponentPtr = std::make_unique<T>();
			ComponentPtr->SetOwner(this);
			m_Components.emplace(ID, std::move(ComponentPtr));
			return static_cast<T*>(m_Components[ID].get());
		}
		
		// Component of this type already exists.
		return nullptr;
	}

	/**
	 * Implementation of GetComponent - Retrieves an attached component by type.
	 */
	template<typename T>
	T* Entity::GetComponent()
	{
		LOG_ASSERT((std::is_base_of<Component, T>::value), "Invalid template parameter! Type T must derive from the Component class.")
		size_t ID = typeid(T).hash_code();
		
		auto It = m_Components.find(ID);
		if (It != m_Components.end())
		{
			It->second.get();
		}

		// Component of this type not found.
		return nullptr;
	}

} // namespace fgl