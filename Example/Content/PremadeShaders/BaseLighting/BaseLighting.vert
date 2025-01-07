#version 410 core     // Ensure this matches the OpenGL version configuration when initializing the window class.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 MVPMatrix;
layout (location = 7) in mat4 ModelMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    FragPos = vec3(ModelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(ModelMatrix))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = MVPMatrix * vec4(aPos, 1.0);
}