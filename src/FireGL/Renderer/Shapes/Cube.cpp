#include <FireGL/Renderer/Shapes/Cube.h>
#include <FireGL/Renderer/Vertex.h>

namespace fgl
{

    Cube::Cube()
        : Shape(
            std::vector<Vertex> {
                    // Front face
                { {-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f} },  // bottom left
                { { 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f} },  // bottom right
                { {-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f} },  // top left
                { { 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f} },  // top right

                    // Back face
                { {-0.5f, -0.5f, 0.5f}, { 0.0f,  0.0f, 1.0f}, {0.0f, 0.0f} },   // bottom left
                { { 0.5f, -0.5f, 0.5f}, { 0.0f,  0.0f, 1.0f}, {1.0f, 0.0f} },   // bottom right
                { {-0.5f,  0.5f, 0.5f}, { 0.0f,  0.0f, 1.0f}, {0.0f, 1.0f} },   // top left
                { { 0.5f,  0.5f, 0.5f}, { 0.0f,  0.0f, 1.0f}, {1.0f, 1.0f} },   // top right

                    // Left face
                { {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },  // bottom left
                { {-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },  // bottom right
                { {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },  // top left
                { {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },  // top right

                    // Right face
                { { 0.5f, -0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },  // bottom left
                { { 0.5f, -0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },  // bottom right
                { { 0.5f,  0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },  // top left
                { { 0.5f,  0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f}, {1.0f, 1.0f} },  // top right

                    // Top face
                { {-0.5f,  0.5f, -0.5f}, { 0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },  // top left
                { { 0.5f,  0.5f, -0.5f}, { 0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },  // top right
                { {-0.5f,  0.5f,  0.5f}, { 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },  // bottom left
                { { 0.5f,  0.5f,  0.5f}, { 0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },  // bottom right

                    // Bottom face
                { {-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f, 0.0f}, {0.0f, 1.0f} }, // top left
                { { 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f, 0.0f}, {1.0f, 1.0f} }, // top right
                { {-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f, 0.0f}, {0.0f, 0.0f} }, // bottom left
                { { 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f, 0.0f}, {1.0f, 0.0f} }, // bottom right
    },
            std::vector<unsigned int> {
            // Front face
            0, 1, 3, 0, 2, 3,

            // Back face
            4, 5, 7, 4, 6, 7,

            // Left face
            8, 9, 11, 8, 10, 11,

            // Right face
            12, 13, 15, 12, 14, 15,

            // Top face
            16, 17, 19, 16, 18, 19,

            // Bottom face
            20, 21, 23, 20, 22, 23
    })
    {
    }

} // namespace fgl