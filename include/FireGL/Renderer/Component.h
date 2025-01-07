#pragma once

namespace fgl
{
	class Entity;

	/**
	 * This class serves as the base class for components in a simple and easy-to-use ECS (Entity-Component-System) framework.
	 *
	 * While not the most performance-optimized approach (due to the lack of CPU cache optimizations and efficient memory layout),
	 * it was designed for ease of use and rapid implementation. If you're looking for a high-performance ECS with CPU caching optimizations,
	 * you should consider using a more advanced solution like EnTT or create your own. This implementation is focused on simplicity
	 * and suitability for rendering purposes, rather than high-performance compute tasks.
	 *
	 * Components derived from this class can be attached to entities, providing functionality such as handling lifecycle events
	 * (e.g., OnBeginPlay, OnTick, OnDestroyed). These methods are meant to be overridden by derived classes to implement specific behavior.
	 */	
	class Component
	{
	public:
		/** Virtual destructor to allow proper cleanup of derived classes. */
		virtual ~Component() = default;

		/**
		 * Called when the owning entity begins play.
		 * Override this in derived classes to initialize component-specific behavior when the entity is first added to the scene.
		 */
		virtual void OnBeginPlay();

		/**
		 * Called every frame to update the component.
		 * Override this in derived classes to perform per-frame updates.
		 *
		 * @param DeltaTime The time that has passed since the last frame.
		 */
		virtual void OnTick(float DeltaTime);

		/**
		 * Called when the entity or the component is destroyed.
		 * Override this in derived classes to handle cleanup when the component is no longer needed.
		 */
		virtual void OnDestroyed();

		/**
		 * Sets the owner entity for this component.
		 * This function is called internally during component creation, and is not intended to be called manually.
		 *
		 * @param Owner Pointer to the entity that owns this component.
		 */
		void SetOwner(Entity* Owner);

	protected:
		/**
		 * Gets the owner entity of this component.
		 * This function is protected to allow derived classes to access the owning entity.
		 *
		 * @return A pointer to the owning entity.
		 */
		Entity* GetOwner() const;

	private:
		Entity* m_Owner; ///< Pointer to the entity that owns this component.
	};

} // namespace fgl