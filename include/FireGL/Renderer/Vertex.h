#pragma once

#include "External/glm/vec2.hpp"
#include "External/glm/vec3.hpp"

namespace fgl
{

	/**
	 * @brief Represents a single vertex in the rendering pipeline.
	 *
	 * Contains position, normal, and texture coordinates used for rendering 3D models.
	 */
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

} // namespace fgl