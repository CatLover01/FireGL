#pragma once

#include <FireGL/fglpch.h>

#include <External/glad/glad.h>
#include <External/glm/glm.hpp>
#include <External/glm/gtc/type_ptr.hpp>

namespace fgl 
{

	/**
	 * Manages an OpenGL shader program, including loading, compiling, and linking shaders.
	 */
	class Shader
	{
	public:
		/**
		 * Constructs a Shader object by compiling and linking shaders from specified file paths.
		 *
		 * @param VertexPath			Path to the vertex shader source file (relative to root, e.g., "src/somefile.vert").
		 * @param FragmentPath			Path to the fragment shader source file (relative to root, e.g., "src/somefile.frag").
		 * @throws std::runtime_error   if any shader file cannot be read or compiled.
		 */
		Shader(std::string_view VertexPath, std::string_view FragmentPath);

		/**
		 * Sets a uniform variable in the shader program.
		 *
		 * These functions individually handle different types of uniforms (bool, int, float, vec3) because OpenGL's C-based API does not support templates. Each type-specific function ensures the correct uniform type is set.
		 *
		 * @param Name   The name of the uniform variable in the shader.
		 * @param Value  The value to assign to the uniform variable.
		 */
		void SetBool(std::string_view Name, bool Value) const;
		void SetInt(std::string_view Name, int Value) const;
		void SetUInt(std::string_view Name, unsigned int Value) const;
		void SetFloat(std::string_view Name, float Value) const;
		void SetDouble(std::string_view Name, double Value) const;
		void SetVec2(std::string_view Name, const glm::vec2& Value) const;
		void SetVec2(std::string_view Name, float X, float Y) const;
		void SetVec3(std::string_view Name, const glm::vec3& Value) const;
		void SetVec3(std::string_view Name, float X, float Y, float Z) const;
		void SetVec4(std::string_view Name, const glm::vec4& Value) const;
		void SetVec4(std::string_view Name, float X, float Y, float Z, float W) const;
		void SetMat2(std::string_view Name, const glm::mat2& Value) const;
		void SetMat3(std::string_view Name, const glm::mat3& Value) const;
		void SetMat4(std::string_view Name, const glm::mat4& Value) const;

		/** Activates the shader program for use in rendering */
		void Activate() const;

		/** Retrieves the ID of the shader program */
		inline uint32_t GetID() const;

	private:
		/**
		 * Retrieves the location of a uniform variable in the shader program.
		 *
		 * Uniform locations are cached for efficiency. Once a uniform location is fetched, it is stored to avoid the slow
		 * glGetUniformLocation call in subsequent frames. This function ensures that uniform setting operations remain optimized.
		 *
		 * @return   The location of the uniform variable, or -1 if the uniform is not found.
		 */		
		GLint GetUniformLocation(std::string_view Name) const;

		/** Loads shader code from a file. */
		std::string LoadShaderCode(std::string_view ShaderPath) const;

		/** Compiles and links vertex and fragment shaders into a program. */
		void CompileAndLinkShaders(const char* VertexCode, const char* FragmentCode);

		/** Compiles a single shader (vertex or fragment). */
		uint32_t CompileShader(const char* ShaderCode, GLenum ShaderType);

		/** Checks for compilation or linking errors in the shader. */
		void CheckCompileErrors(unsigned int Shader, std::string_view Type);

	private:
		/** OpenGL Program ID */
		uint32_t m_ID;

		/** 
		 * Cached Uniform Location map for optimization.
		 * 
		 * This cache stores uniform locations for each uniform by name.
		 * Storing these locations avoids the need to repeatedly call glGetUniformLocation during the application lifetime, which can be slow.
		 * Using a cache improves performance, particularly in real-time rendering where uniforms are set frequently.
		 */
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
	};

} // namespace fgl