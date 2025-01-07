#version 410 core        // Ensure this matches the OpenGL version configuration when initializing the window class.

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}