#include <FireGL/Renderer/Shapes/Sphere.h>
#include <FireGL/Renderer/Vertex.h>

#include <numbers>

namespace fgl
{

    Sphere::Sphere(float Radius, int Stacks, int Slices)
        : Shape(std::move(GenerateVertices(Radius, Stacks, Slices)), std::move(GenerateIndices(Stacks, Slices)))
    {
    }

    std::vector<Vertex> Sphere::GenerateVertices(float Radius, int Stacks, int Slices)
    {
        std::vector<Vertex> Vertices;

        // Loop through latitudinal segments (Stacks)
        for (int StackIndex = 0; StackIndex <= Stacks; ++StackIndex) {
            // Calculate the vertical angle (phi), ranging from 0 to Pi
            float Phi = std::numbers::pi * StackIndex / Stacks;

            // Loop through longitudinal segments (Slices)
            for (int SliceIndex = 0; SliceIndex <= Slices; ++SliceIndex) {
                // Calculate the horizontal angle (theta), ranging from 0 to 2*Pi
                float Theta = 2.0f * std::numbers::pi * SliceIndex / Slices;

                // Convert spherical coordinates to Cartesian coordinates
                float X = Radius * sinf(Phi) * cosf(Theta);
                float Y = Radius * cosf(Phi);
                float Z = Radius * sinf(Phi) * sinf(Theta);

                glm::vec3 Position(X, Y, Z);
                glm::vec3 Normal = glm::normalize(Position);

                // Calculate texture coordinates (u, v) based on the current slice and stack
                glm::vec2 TexCoords = { 
                    static_cast<float>(SliceIndex) / Slices,
                    static_cast<float>(StackIndex) / Stacks 
                };

                // Store the vertex data (Position, Normal, TexCoords)
                Vertices.push_back({ Position, Normal, TexCoords });
            }
        }
        return Vertices;
    }

    std::vector<unsigned int> Sphere::GenerateIndices(int Stacks, int Slices)
    {
        std::vector<unsigned int> Indices;

        // Loop through each stack and slice to generate indices for triangles
        for (int StackIndex = 0; StackIndex < Stacks; ++StackIndex) {
            for (int SliceIndex = 0; SliceIndex < Slices; ++SliceIndex) {
                // Calculate indices for the current quad
                int First = (StackIndex * (Slices + 1)) + SliceIndex;
                int Second = First + Slices + 1;

                // Create two triangles for each quad (quad split into 2 tris)
                Indices.push_back(First);        // Triangle 1
                Indices.push_back(Second);
                Indices.push_back(First + 1);

                Indices.push_back(Second);       // Triangle 2
                Indices.push_back(Second + 1);
                Indices.push_back(First + 1);
            }
        }
        return Indices;
    }

} // namespace fgl