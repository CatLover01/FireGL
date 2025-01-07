#include <FireGL/Renderer/Model.h>
#include <FireGL/Core/BaseLog.h>
#include <FireGL/Renderer/Material.h>
#include <FireGL/Renderer/Transform.h>
#include <FireGL/Renderer/Mesh.h>

#include <External/stb/stb_image.h>

#include <filesystem>

namespace fgl
{

	Model::Model(std::string_view Path)
		: SceneObject()
	{
		LoadModel(Path);
	}

	std::vector<BaseMesh>& Model::GetMeshes()
	{
		return m_Meshes;
	}

	size_t Model::GetHash() const
	{
		return m_Meshes[0].GetMeshHash();
	}

	void Model::Render(size_t NumberInstance) const
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i].Render(NumberInstance);
		}
	}

	void Model::BeginPlay()
	{
	}

	void Model::Tick(float DeltaTime)
	{
	}

	void Model::Destroy()
	{
		CleanupTextures();
		m_Meshes.clear();
		m_CachedTextures.clear();
	}

	std::string Model::GetTextureNumber(std::string_view Name, unsigned int& DiffuseNr, unsigned int& SpecularNr)
	{
		if (Name == "texture_diffuse")
			return std::to_string(DiffuseNr++);
		if (Name == "texture_specular")
			return std::to_string(SpecularNr++);
		return "0";
	}

	void Model::SetMaterial(std::shared_ptr<Material> Material)
	{
		if (!Material)
		{
			LOG_ERROR("Failed to set material: Provided material is nullptr. Ensure a valid material is passed to the function.", true)
			return;
		}

		for (BaseMesh& Mesh : m_Meshes)
		{
			Mesh.SetMaterial(Material);
		}

		Material->SetSceneObject(this);
	}

	const std::shared_ptr<Material> Model::GetMaterial(size_t MeshIndex) const
	{
		if (m_Meshes.empty() || MeshIndex >= m_Meshes.size())
		{
			LOG_INFO("No mesh data was found in the model when attempting to retrieve its material. \
				Ensure that the model has at least one mesh before calling GetMaterial().")
			return nullptr;
		}

		return m_Meshes[MeshIndex].GetMaterial();
	}

	void Model::LoadModel(std::string_view Path)
	{
		Assimp::Importer Import;
		const aiScene* Scene = Import.ReadFile(Path.data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

		if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
		{
			LOG_ERROR("Assimp Error: " + std::string(Import.GetErrorString()), true);
		}

		m_Directory = Path.substr(0, Path.find_last_of('/'));
		ProcessNode(Scene->mRootNode, Scene);
	}

	void Model::ProcessNode(aiNode* Node, const aiScene* Scene)
	{
		ProcessNodeMesh(Node, Scene);
		ProcessNodeChild(Node, Scene);
	}

	void Model::ProcessNodeMesh(aiNode* Node, const aiScene* Scene)
	{
		for (unsigned int i = 0; i < Node->mNumMeshes; i++)
		{
			aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
			// Only compute hash for the first mesh (i == false)
			m_Meshes.push_back(ProcessMesh(Mesh, Scene, i));
		}
	}

	void Model::ProcessNodeChild(aiNode* Node, const aiScene* Scene)
	{
		for (unsigned int i = 0; i < Node->mNumChildren; i++)
		{
			ProcessNode(Node->mChildren[i], Scene);
		}
	}

	BaseMesh Model::ProcessMesh(aiMesh* Mesh, const aiScene* Scene, bool ComputeHash)
	{
		return BaseMesh(std::move(ProcessVertices(Mesh)), std::move(ProcessIndices(Mesh)), std::move(ProcessTextures(Mesh, Scene)), !ComputeHash);
	}

	std::vector<Vertex> Model::ProcessVertices(aiMesh* Mesh)
	{
		std::vector<Vertex> Vertices;

		for (unsigned int i = 0; i < Mesh->mNumVertices; i++)
		{
			Vertices.push_back(CreateVertex(Mesh, i));
		}
		return Vertices;
	}

	Vertex Model::CreateVertex(aiMesh* Mesh, unsigned int Index)
	{
		Vertex Vertex;
		Vertex.Position = glm::vec3(Mesh->mVertices[Index].x, Mesh->mVertices[Index].y, Mesh->mVertices[Index].z);
		Vertex.Normal = glm::vec3(Mesh->mNormals[Index].x, Mesh->mNormals[Index].y, Mesh->mNormals[Index].z);
		if (Mesh->mTextureCoords[0])
		{
			Vertex.TexCoords = glm::vec2(Mesh->mTextureCoords[0][Index].x, Mesh->mTextureCoords[0][Index].y);
		}
		return Vertex;
	}

	std::vector<unsigned int> Model::ProcessIndices(aiMesh* Mesh)
	{
		std::vector<unsigned int> Indices;

		for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
		{
			const aiFace& Face = Mesh->mFaces[i];
			Indices.insert(Indices.end(), Face.mIndices, Face.mIndices + Face.mNumIndices);
		}
		return Indices;
	}

	std::vector<Texture> Model::ProcessTextures(aiMesh* Mesh, const aiScene* Scene)
	{
		std::vector<Texture> Textures;

		if (Mesh->mMaterialIndex >= 0)
		{
			aiMaterial* Material = Scene->mMaterials[Mesh->mMaterialIndex];
			AddMaterialTextures(Material, aiTextureType_DIFFUSE, "texture_diffuse", Textures);
			AddMaterialTextures(Material, aiTextureType_SPECULAR, "texture_specular", Textures);
		}
		return Textures;
	}

	void Model::AddMaterialTextures(aiMaterial* Material, aiTextureType Type, std::string_view TypeName, std::vector<Texture>& Textures)
	{
		for (unsigned int i = 0; i < Material->GetTextureCount(Type); i++)
		{
			AddTexture(Material, Type, TypeName, i, Textures);
		}
	}

	void Model::AddTexture(aiMaterial* Material, aiTextureType Type, std::string_view TypeName, unsigned int Index, std::vector<Texture>& Textures)
	{
		aiString Str;
		Material->GetTexture(Type, Index, &Str);

		if (IsTextureLoaded(Str))
		{
			Textures.push_back(FindLoadedTexture(Str));
		}
		else
		{
			Textures.push_back(LoadNewTexture(Str, TypeName));
		}
	}

	bool Model::IsTextureLoaded(const aiString& str)
	{
		return std::any_of(m_CachedTextures.begin(), m_CachedTextures.end(), [&str](const Texture& Texture) {
			return std::strcmp(Texture.GetPath().data(), str.C_Str()) == 0;
			});
	}

	Texture Model::FindLoadedTexture(const aiString& str)
	{
		auto It = std::find_if(m_CachedTextures.begin(), m_CachedTextures.end(), [&str](const Texture& Texture) {
			return std::strcmp(Texture.GetPath().data(), str.C_Str()) == 0;
			});
		return *It;
	}

	Texture Model::LoadNewTexture(const aiString& str, std::string_view TypeName)
	{
		Texture Texture;
		std::filesystem::path FilePath = std::filesystem::path(m_Directory) / str.C_Str();
		Texture.LoadTexture(FilePath.string());
		Texture.SetName(TypeName);
		Texture.SetPath(str.C_Str());
		m_CachedTextures.push_back(Texture);
		return Texture;
	}

	void Model::CleanupTextures()
	{
		CleanupMeshTextures();
		CleanupCachedTextures();
	}

	void Model::CleanupMeshTextures()
	{
		for (auto& Mesh : m_Meshes)
		{
			for (auto& Texture : Mesh.GetTextures())
			{
				Texture.Cleanup();
			}
		}
	}

	void Model::CleanupCachedTextures()
	{
		for (auto& Texture : m_CachedTextures)
		{
			Texture.Cleanup();
		}
	}

} // namespace fgl