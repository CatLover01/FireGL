#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Renderer/Vertex.h>
#include <FireGL/Renderer/Texture.h>

#include <External/glm/mat4x4.hpp>

namespace fgl
{
	class Material;

	/**
	 * @class BaseMesh
	 *
	 * @brief A class that encapsulates the core functionality for rendering a 3D mesh with OpenGL.
	 *        It manages vertex and index data, sets up vertex attributes, and handles the rendering of
	 *        the mesh with optional material support. This class is not intended to be derived from, as
	 *        it controls its own internal state and OpenGL resources.
	 *
	 * @details The class provides methods for:
	 *          - Initializing and configuring buffers (First and Second Pass)
	 *          - Binding textures and materials to the mesh
	 *          - Rendering the mesh with instancing support
	 *          - Computing and retrieving a unique mesh hash for efficient instanced rendering
	 *          - Storing vertex, index, and texture data for mesh rendering
	 */
	class BaseMesh
	{
	public:
		/**
		 * Constructs a BaseMesh with vertices, indices, textures, and a flag to compute hash.
		 * @param Vertices The vertices of the mesh.
		 * @param Indices The indices of the mesh.
		 * @param Textures The textures associated with the mesh.
		 * @param bComputeHash Flag indicating whether to compute the mesh hash.
		 */
		BaseMesh(std::vector<Vertex>&& Vertices, std::vector<unsigned int>&& Indices, std::vector<Texture>&& Textures, bool bComputeHash);
		
		/**
		 * Performs the first pass of mesh setup by initializing buffers and configuring vertex attributes.
		 */
		void FirstPass();

		/**
		 * Performs the second pass of mesh setup by configuring vertex attributes for instancing.
		 */
		void SecondPass();
		
		/**
		 * Retrieves the mesh hash used for automatic instanced rendering.
		 * @return The hash of the mesh.
		 */
		size_t GetMeshHash() const;

		/**
		 * Renders the mesh with the specified number of instances.
		 * @param NumberInstance The number of instances to render.
		 */
		void Render(size_t NumberInstance) const;

		/**
		 * Sets the material for this mesh.
		 * @param Material The material to be applied to the mesh.
		 */
		void SetMaterial(std::shared_ptr<Material> Material);

		/**
		 * Gets the current material of the mesh.
		 * @return The current material applied to the mesh.
		 */
		const std::shared_ptr<Material> GetMaterial() const;

		/**
		 * Gets the textures associated with the mesh.
		 * @return A reference to the vector of textures.
		 */
		std::vector<Texture>& GetTextures();

	private:
		/**
		 * Initializes the vertex buffers (VAO, VBO, EBO).
		 */
		void InitializeBuffers();

		/**
		 * Configures the vertex attributes for the mesh (e.g., position, normal, texture coordinates).
		 */
		void ConfigureVertexAttributes();

		/**
		 * Configures the vertex attributes for instancing (e.g., model matrices).
		 */
		void ConfigureVertexAttributesInstances();

		/**
		 * Computes a hash for the mesh based on its vertices and indices.
		 */
		void ComputeHash();

	private:
		std::vector<Vertex>		    m_Vertices; ///< Vertices of the mesh.
		std::vector<unsigned int>   m_Indices;  ///< Indices of the mesh.
		std::vector<Texture>		m_Textures; ///< Textures associated with the mesh.
		std::shared_ptr<Material>   m_Material; ///< Material applied to the mesh.
		size_t m_MeshHash;						///< Hash value for the mesh.

		GLuint VAO, VBO, EBO;				    ///< OpenGL buffers (Vertex Array, Vertex Buffer, Element Buffer).
	};

} // namespace fgl