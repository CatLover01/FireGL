#pragma once

#include <FireGL/Renderer/Entity.h>

namespace fgl
{

	/** 
	 * Simple Skybox class derived from Entity to demonstrate what can be done with custom rendering logic
	 */
	class SkyboxEntity : public Entity
	{
	public:
		using Entity::Entity;

		virtual bool IsSkybox() const override { return true; }

	protected:
		virtual void OnPrepareRender() const override
		{
			// Skybox will be rendered behind other objects.
			glDepthFunc(GL_LEQUAL);
		}

		virtual void OnPostRender() const override
		{
			// Restores the default depth function for further rendering.
			glDepthFunc(GL_LESS);
		}
	};

} // namespace fgl