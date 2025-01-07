#pragma once

#include <FireGL/fglpch.h>

#include <External/glm/mat4x4.hpp>

namespace fgl
{
	class Shader;
	class Texture;
	class SceneObject;

	/**
	 * @class Material
	 *
	 * @brief Represents a material used in rendering, combining a shader and textures to define
	 *        the visual properties of a surface and how it interacts with light in a 3D scene.
	 *
	 * @details This class serves as the foundation for creating materials that affect the appearance
	 *          of objects in the scene. It integrates shaders with textures, allowing for customized
	 *          surface rendering. Derived classes can extend the functionality by adding custom logic
	 *          for handling uniforms, providing flexibility for different material types and effects.
	 */
	class Material
	{
	public:
		/**
		 * Constructs a Material object and initializes the shader for the material.
		 * This constructor must be called by any derived class to ensure the material
		 * has a shader program.
		 *
		 * @param Shader    A pointer to the Shader object to be used by the material.
		 */
		Material(Shader* Shader);

		/**
		 * Associates a texture with the material, using the given texture name.
		 * The maximum number of textures that can be bound to a material is 32.
		 *
		 * @param TextureName    The name of the texture (e.g., "diffuse", "specular").
		 * @param Texture        A pointer to the Texture object to be associated.
		 */
		void SetTexture(std::string_view TextureName, Texture* Texture);

		/**
		 * Activates the material, binding all associated textures and the shader.
		 * This function is called by the renderer to prepare the material for use
		 * during rendering.
		 */
		void Activate();

		/**
		 * Sets the SceneObject that the material is applied to, ensuring the material
		 * has access to the camera MVP (Model-View-Projection) for proper rendering.
		 * This function is called internally and should not be called manually.
		 *
		 * @param SceneObject    A pointer to the SceneObject the material is applied to.
		 */		
		void SetSceneObject(SceneObject* SceneObject);

		/**
		 * Retrieves the shader currently associated with the material.
		 * This function is used in the derived class' 'ApplyUniforms' method
		 * to access and apply uniforms to the shader, including texture samplers and transformation matrices.
		 *
		 * @return A pointer to the Shader object used by the material.
		 */
		const Shader* GetShader() const;

	protected:
		/**
		 * This function is intended to be overridden by derived classes to apply custom
		 * uniforms to the shader (e.g., MVP matrices). It provides a default implementation
		 * but can be customized for specific needs.
		 */
		virtual void ApplyUniforms();

		/**
		 * Retrieves the texture associated with the given name.
		 * This function is typically used in the derived class' 'ApplyUniforms' method
		 * to get the texture data and send it to the shader for proper rendering.
		 *
		 * @param TextureName    The name of the texture to retrieve (e.g., "diffuse", "normal").
		 * @return               A pointer to the Texture object, or nullptr if the texture is not found.
		 */
		const Texture* GetTexture(std::string_view TextureName) const;

		/**
		 * Retrieves the SceneObject to which the material is applied.
		 * This function is used in the derived class' 'ApplyUniforms' method to
		 * allow access to the object's transformation data, which is necessary to calculate the
		 * Model-View-Projection (MVP) matrix for the shader. It can also be used to get the camera for custom shader logic.
		 *
		 * @return A pointer to the associated SceneObject, which provides the necessary transformation data.
		 */
		SceneObject* GetSceneObject() const;

	private:
		/**
		 * Activates all textures associated with the material, binding them to the appropriate
		 * texture units for use in the shader.
		 */
		void ActivateTextures() const;

	private:
		Shader* m_ShaderProgram;							  ///< A pointer to the shader program used by the material
		std::unordered_map<std::string, Texture*> m_Textures; ///< A map of texture names to texture pointers

		SceneObject* m_SceneObject;							  ///< A pointer to the SceneObject this material is applied to
	};

} // namespace fgl