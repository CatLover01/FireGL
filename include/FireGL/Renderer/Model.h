#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Core/BaseLog.h>
#include <FireGL/Renderer/SceneObject.h>
#include <FireGL/Renderer/Mesh.h>
#include <FireGL/Renderer/Vertex.h>
#include <FireGL/Renderer/Texture.h>
#include <FireGL/Renderer/Shader.h>

#include <External/assimp/Importer.hpp>
#include <External/assimp/scene.h>
#include <External/assimp/postprocess.h>

namespace fgl
{
	class Material;
	class Transform;
	class BaseMesh;

	/**
	 * Model Importer Class
	 *
	 * Overview:
	 * This class is designed solely as a utility for importing 3D models using Assimp.
	 * - **Non-Derivable:** This class is not intended to be inherited or extended.
	 * - **Usage:** Imported models should be attached to an entity or actor for further usage in the scene.
	 *   For example, you can encapsulate this model inside a scene object with added functionality.
	 *
	 * Key Notes:
	 * - Importing models can be slow, taking 5 to 50 seconds depending on model size (5 MB = ~5s, 25 MB = ~50s).
	 * - Ensure that the model's object file (.obj, .fbx, etc.) is in the same directory as its textures for proper loading.
	 * - While this class can be rendered directly, creating an entity for it is recommended for additional flexibility.
	 */
	class Model : public SceneObject
	{
	public:
		/**
		  * Constructs a Model object and initiates loading from the specified file path.
		  *
		  * @param Path The file path of the model to load.
		  */
		Model(std::string_view Path);

		/**
		 * Sets up the material for the model using the provided shader.
		 * This function creates a material of type T (which must be derived from Material)
		 * and binds textures to the material using the provided shader.
		 * 
		 * This function must be called after the constructor to ensure that the material is valid
		 * and usable with the specified shader.
		 *
		 * @tparam T The type of material, which must be derived from the Material class.
		 * @param Shader The shader to be used for the material.
		 * @return A shared pointer to the material of type T created for this model.
		 */
		template<typename T>
		std::shared_ptr<Material> SetupMaterial(Shader* Shader);

		/**
		 * Renders the model.
		 * This function binds the necessary resources and draws the model meshes to the screen.
		 */
		virtual void Render(size_t NumberInstance) const override final;

		/**
		 * The following functions are unused in this context as this class is an importer
		 * and is not meant to be further derived. For extended functionality, consider using an entity.
		 */
		virtual void Tick(float DeltaTime) override final;
		virtual void BeginPlay() override final;
		virtual void Destroy() override final;

		/**
		 * Sets the material for the model, replacing the current material.
		 * This will override the model's attached textures, so use with caution.
		 *
		 * @param Material A shared pointer to the new material to be assigned to the model.
		 */
		virtual void SetMaterial(std::shared_ptr<Material> Material) override final;

		/**
		 * Retrieves the material assigned to the model.
		 * For models with multiple meshes, specify MeshIndex to get the material for a particular mesh.
		 * Defaults to the first mesh (index 0). Returns nullptr if the index is invalid.
		 *
		 * @param MeshIndex The index of the mesh whose material is being retrieved.
		 * @return A shared pointer to the material assigned to the model.
		 */
		virtual const std::shared_ptr<Material> GetMaterial(size_t MeshIndex = 0) const override final;

		/**
		 * Retrieves the meshes that make up the model.
		 *
		 * @return A reference to the vector of meshes.
		 */
		virtual std::vector<BaseMesh>& GetMeshes() override;

		/**
		 * Retrieves the hash used for batching the model's vertex objects.
		 * The hash is calculated based on the vertex data and is used to group together
		 * identical vertex objects for efficient instanced rendering.
		 *
		 * @return The hash value used for batching the model's vertex objects.
		 */
		virtual size_t GetHash() const override;

	private:
		/**
		 * Loads the model from the specified file path using Assimp.
		 * This function processes the scene root node.
		 */
		void LoadModel(std::string_view Path);

		/**
		 * Processes a node in the Assimp scene graph.
		 * Recursively processes all child nodes and their meshes.
		 */
		void ProcessNode(aiNode* Node, const aiScene* Scene);
		void ProcessNodeMesh(aiNode* Node, const aiScene* Scene);
		void ProcessNodeChild(aiNode* Node, const aiScene* Scene);

		/**
		 * Processes a single mesh and converts it into a format suitable for rendering.
		 * If the ComputeHash flag is false, the mesh will be hashed; otherwise, the hash will not be computed.
		 *
		 * @param Mesh The Assimp mesh to be processed.
		 * @param Scene The Assimp scene.
		 * @param ComputeHash Flag to determine if the mesh should be hashed.
		 *                   If true, the mesh will not be hashed; if false, the mesh will be hashed.
		 * @return A BaseMesh containing the processed mesh data.
		 */
		inline BaseMesh ProcessMesh(aiMesh* Mesh, const aiScene* Scene, bool ComputeHash);

		/** Loads textures associated with the imported material. */
		std::vector<Texture> LoadMaterialTextures(aiMaterial* Material,aiTextureType Type, std::string TypeName);

		/** Converts Assimp vertex data into a vector of custom Vertex structures. */
		std::vector<Vertex> ProcessVertices(aiMesh* Mesh);

		/**
		 * Creates a custom Vertex structure from a specific index in the mesh.
		 * Extracts the position, normal, texture coordinates, and other vertex attributes.
		 */
		Vertex CreateVertex(aiMesh* Mesh, unsigned int Index);

		/** Extracts the index data used to define the faces of the mesh. */
		std::vector<unsigned int> ProcessIndices(aiMesh* Mesh);

		/** Retrieves the textures associated with the mesh from the material. */
		std::vector<Texture> ProcessTextures(aiMesh* Mesh, const aiScene* Scene);

		/**
		 * Utility and helper functions related to textures and materials.
		 * These functions assist with texture loading, binding, and managing material-specific textures.
		 */
		void AddMaterialTextures(aiMaterial* Material, aiTextureType Type, std::string_view TypeName, std::vector<Texture>& Textures);
		void AddTexture(aiMaterial* Material, aiTextureType Type, std::string_view TypeName, unsigned int Index, std::vector<Texture>& Textures);
		bool IsTextureLoaded(const aiString& str);
		Texture FindLoadedTexture(const aiString& str);
		Texture LoadNewTexture(const aiString& str, std::string_view TypeName);
		std::string GetTextureNumber(std::string_view Name, unsigned int& DiffuseNr, unsigned int& SpecularNr);
		template<typename T>
		void BindTexturesToMaterial(Shader* Shader, const std::shared_ptr<T>& LightingMat);
		template<typename T>
		void BindTexture(Shader* Shader, const std::shared_ptr<T>& LightingMat,
			Texture& Texture, unsigned int& DiffuseNr, unsigned int& SpecularNr);

		/**
		 * Cleanup functions to manage resource deallocation.
		 * Ensures that textures and cached data are properly cleaned up.
		 */
		void CleanupTextures();
		void CleanupMeshTextures();
		void CleanupCachedTextures();

	private:
		std::vector<BaseMesh> m_Meshes;			///< A collection of meshes that make up the model.
		std::string m_Directory;				///< Directory containing the path to the imported model.
		std::vector<Texture> m_CachedTextures;  ///< A cache of loaded textures to avoid duplicate loading.
	};

	/**
	 * Implementation of SetupMaterial for creating a material using the specified shader and binding textures to it.
	 */
	template<typename T>
	std::shared_ptr<Material> Model::SetupMaterial(Shader* Shader)
	{
		LOG_ASSERT((std::is_base_of<Material, T>::value), "T must be derived from Material")
		if (!Shader)
		{
			LOG_ERROR("Failed to set up material: Provided shader is nullptr. Ensure a valid shader is passed to the function.", true)
			return nullptr;
		}

		std::shared_ptr<T> LightingMat = std::make_shared<T>(Shader);
		BindTexturesToMaterial(Shader, LightingMat);

		SetMaterial(LightingMat);
		return LightingMat;
	}

	/**
	 * Implementation of BindTexturesToMaterial for binding all textures of the model to the provided material and shader.
	 */
	template<typename T>
	void Model::BindTexturesToMaterial(Shader* Shader, const std::shared_ptr<T>& LightingMat)
	{
		unsigned int DiffuseNr = 1, SpecularNr = 1;

		for (BaseMesh& Mesh : m_Meshes)
		{
			for (Texture& Texture : Mesh.GetTextures())
			{
				BindTexture(Shader, LightingMat, Texture, DiffuseNr, SpecularNr);
			}
		}
	}

	/**
	 * Implementation of BindTexture for binding a single texture to the material and setting its uniform in the shader.
	 */
	template<typename T>
	void Model::BindTexture(Shader* Shader, const std::shared_ptr<T>& LightingMat, Texture& Texture, unsigned int& DiffuseNr, unsigned int& SpecularNr)
	{
		Texture.Activate();
		std::string Number = GetTextureNumber(Texture.GetName(), DiffuseNr, SpecularNr);

		// Set the shader uniform for this texture (e.g., material.roughness0 / material.diffuse1).
		// The uniform name is generated based on the texture's semantic type and number (e.g., diffuse or specular).
		Shader->SetInt(("material." + Texture.GetName() + Number).c_str(), Texture.GetSlotIndex());
		LightingMat->SetTexture(Texture.GetName() + Number, &Texture);
	}

} // namespace fgl