#pragma once

#include <FireGL/Renderer/Material.h>

namespace fgl
{
	class Shader;

	/** 
	 * This class customizes the material for the skybox, ensuring it is rendered as infinite by 
	 * manually applying the projection and view matrices without model transformations.
	 */
	class SkyboxMaterial : public Material
	{
	public:
		SkyboxMaterial(Shader* Shader);

	protected:
		virtual void ApplyUniforms() override;
	};

} // namespace fgl