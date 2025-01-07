#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Renderer/Shapes/Shape.h>

namespace fgl
{
	class Vertex;

	/**
	 * A sphere shape defined by a radius, stacks, and slices, providing a smooth, rounded surface.
	 * The sphere is constructed with a specified radius and resolution, where 'Stacks' determine the number
	 * of horizontal segments (latitude), and 'Slices' determine the number of vertical segments (longitude).
	 * This allows for adjustable levels of detail for rendering or collision purposes.
	 *
	 * @param Radius  The radius of the sphere. Defaults to 1.0f.
	 * @param Stacks  The number of horizontal segments (latitude). Defaults to 36.
	 * @param Slices  The number of vertical segments (longitude). Defaults to 18.
	 */
	class Sphere : public Shape
	{
	public:
		Sphere(float Radius = 1.f, int Stacks = 36, int Slices = 18);

	private:
		std::vector<Vertex> GenerateVertices(float Radius, int Stacks, int Slices);
		std::vector<unsigned int> GenerateIndices(int Stacks, int Slices);
	};

} // namespace fgl