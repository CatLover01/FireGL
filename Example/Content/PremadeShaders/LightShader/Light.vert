#version 410 core     // Ensure this matches the OpenGL version configuration when initializing the window class.
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 MVPMatrix;

void main()
{
	gl_Position = MVPMatrix * vec4(aPos, 1.0);
}