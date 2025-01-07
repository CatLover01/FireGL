#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Renderer/MatrixBuffer.h>

#include <External/glm/mat4x4.hpp>

namespace fgl
{
	class Scene;
	class SceneObject;

	/**
	 * Enumeration representing different rendering modes.
	 */
	enum class RenderingMode
	{
		Default,   ///< Standard rendering mode
		DebugLine, ///< Renders object outlines as lines
		DebugFill  ///< Renders object meshes with solid fill for debugging
	};

	/**
	 * Renderer class responsible for rendering a Scene using various rendering modes.
	 *
	 * This class manages drawing objects in a Scene and updating Model-View-Projection (MVP)
	 * matrices for instanced rendering. It supports different rendering modes, such as Default,
	 * Debug Line, and Debug Fill, enabling visualization and debugging options.
	 */
	class Renderer
	{
	public:
		/**
		 * Constructs a Renderer and configures it based on the provided rendering mode.
		 *
		 * @param Mode Specifies the rendering mode to initialize with (default is RenderingMode::Default).
		 */
		Renderer(RenderingMode Mode = RenderingMode::Default);

		/** Destructor responsible for freeing allocated buffers and resources. */
		~Renderer();

		/**
		 * Renders all objects within the given Scene by drawing batches of instanced objects.
		 *
		 * This function clears the screen, processes the Scene to group objects by mesh,
		 * updates transformations, and performs instanced rendering.
		 *
		 * @param TargetScene A pointer to the Scene to render.
		 */
		void Render(Scene* Scene);
		
		/**
		 * Updates the rendering mode dynamically.
		 *
		 * This function allows the user to switch between rendering modes at runtime,
		 * reconfiguring OpenGL states as necessary.
		 *
		 * @param NewMode The rendering mode to switch to (e.g., DebugLine or Default).
		 */		
		void ConfigureRenderingMode(RenderingMode NewMode);

	private:
		/**
		 * Sets up the OpenGL buffer used to store MVP matrices.
		 *
		 * This function generates the buffer and prepares it for instanced rendering.
		 */
		void SetupBuffer();

		/** Deletes the OpenGL buffer to prevent memory leaks. */
		void CleanupBuffer();

		/** Clears the screen's color and depth buffers before rendering each frame. */
		void ClearFrameBuffer();

		/**
		 * Groups Scene objects by their vertex data, preparing them for batch rendering.
		 *
		 * @param TargetScene The Scene to process.
		 * @param Skybox Reference to a pointer that stores the skybox object (if present).
		 * @return A hashmap that batches SceneObjects by their vertex hash.
		 */
		std::map<size_t, std::vector<SceneObject*>> BatchSceneObjects(Scene* Scene, SceneObject*& Skybox);
		
		/**
		 * Renders batches of Scene objects using instanced rendering.
		 * This method loops through each batch and renders them efficiently.
		 *
		 * @param ObjectBatches A hashmap containing grouped Scene objects ready for rendering.
		 */
		void RenderBatches(const std::map<size_t, std::vector<SceneObject*>>& ObjectBatches);
		
		/**
		 * Renders the skybox object separately from other Scene objects.
		 *
		 * Skybox rendering uses a single instance to ensure it is drawn last, providing
		 * background visuals.
		 *
		 * @param Skybox Pointer to the Scene object representing the skybox.
		 */
		void RenderSkybox(SceneObject* Skybox);

		/**
		 * Updates MVP matrices for all batched objects in the Scene.
		 *
		 * This function calculates and uploads MVP matrices for instanced rendering.
		 * If the buffer isn't large enough, it resizes it accordingly.
		 *
		 * @param TargetScene The Scene being rendered.
		 * @param ObjectBatches Batches of Scene objects to update.
		 * @param HasSkybox Indicates if the Scene contains a skybox, adjusting matrix allocation.
		 */
		void UpdateMVPInstances(const Scene* Scene, const std::map<size_t, std::vector<SceneObject*>>& ObjectBatches, bool HasSkybox);
		
		/**
		 * Ensures the MVP buffer has sufficient capacity to store all matrices.
		 *
		 * If the current buffer size is insufficient, it resizes the buffer to accommodate
		 * all Scene objects, avoiding frequent reallocation.
		 *
		 * @param TotalObjectCount The number of objects requiring MVP matrix storage.
		 */
		void EnsureBufferCapacity(size_t TotalObjectCount);

		/**
		 * Computes and stores the MVP matrix for a single Scene object.
		 *
		 * This function updates the transformation matrices and uploads them to the MVP buffer.
		 *
		 * @param Object Pointer to the Scene object being processed.
		 * @param Index Reference to the buffer index, incremented as matrices are stored.
		 */
		void ProcessObjectForMVP(SceneObject* Object, size_t& Index);

		/** Binds the MVP buffer to the OpenGL pipeline, preparing for data transfer. */
		void BindMVPBuffer();

		/**
		 * Performs the first rendering pass for a Scene object.
		 * The first pass prepares vertex and texture data before instanced rendering.
		 *
		 * @param Object Pointer to the Scene object being processed.
		 */
		void PerformFirstPass(SceneObject* Object);

		/**
		 * Executes the second pass to finalize object rendering.
		 * This pass handles final transformations and GPU state changes.
		 *
		 * @param Object Pointer to the Scene object being processed.
		 */
		void PerformSecondPass(SceneObject* Object);

		/** Transfers updated MVP matrices to the GPU for instanced rendering. */
		void UploadMVPDataToGPU();

	private:
		unsigned int m_MVPBufferID;		 ///< OpenGL buffer ID for storing MVP matrices
		MatrixBuffer m_MVPMatrixBuffer;  ///< Buffer to store Model-View-Projection matrices for instanced rendering
	};

} // namespace fgl