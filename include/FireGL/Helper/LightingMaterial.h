#pragma once

#include <FireGL/Renderer/Material.h>

namespace fgl
{
	class Shader;

	/**
	 * This class handles the application of lighting uniforms, including directional, point, and spotlights,
	 * as well as camera position and material properties for proper shading in the scene.
	 */
	class LightingMaterial : public Material
	{
	public:
		LightingMaterial(Shader* Shader);

	protected:
		virtual void ApplyUniforms() override;
	};

} // namespace fgl