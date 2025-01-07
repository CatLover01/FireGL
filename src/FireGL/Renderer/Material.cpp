#include <FireGL/Renderer/Material.h>
#include <FireGL/Core/BaseLog.h>
#include <FireGL/Renderer/Shader.h>
#include <FireGL/Renderer/Texture.h>
#include <FireGL/Renderer/SceneObject.h>

namespace fgl
{

	Material::Material(Shader* Shader)
		: m_ShaderProgram{ Shader }
	{
	}

	void Material::SetTexture(std::string_view TextureName, Texture* Texture)
	{
		LOG_ASSERT(Texture, "Texture initialized was not valid...")
		
		Texture->SetSlotIndex(m_Textures.size());
		m_Textures[TextureName.data()] = Texture;
	}

	void Material::Activate()
	{
		if (!m_ShaderProgram)
		{
			LOG_ERROR("Shader program is null or uninitialized. Ensure a valid shader program is assigned to this material before activation.", true)
			return;
		}

		m_ShaderProgram->Activate();
		ActivateTextures();
		ApplyUniforms();
	}

	void Material::SetSceneObject(SceneObject* SceneObject)
	{
		m_SceneObject = SceneObject;
	}

	const Texture* Material::GetTexture(std::string_view TextureName) const
	{
		auto it = m_Textures.find(TextureName.data());
		if (it != m_Textures.end())
		{
			return it->second;
		}
		else
		{
			LOG_ERROR("Failed to retrieve texture: \"" + std::string(TextureName) + "\". Ensure the texture name is correct and matches the expected value.", true)
			return nullptr;
		}
	}

	const Shader* Material::GetShader() const
	{
		return m_ShaderProgram;
	}

	void Material::ApplyUniforms()
	{
	}

	SceneObject* Material::GetSceneObject() const
	{
		return m_SceneObject;
	}

	void Material::ActivateTextures() const
	{
		for (const auto& [TextureName, Texture] : m_Textures)
		{
			if (Texture)
			{
				Texture->Activate();
			}
			else
			{
				LOG_INFO("Texture with name '" + TextureName + "' is null or uninitialized. Skipping activation.")
			}
		}
	}

} // namespace fgl