#include <FireGL/Renderer/Shader.h>
#include <FireGL/Core/BaseLog.h>

namespace fgl {

	Shader::Shader(std::string_view VertexPath, std::string_view FragmentPath)
	{
		std::string VertexCode = LoadShaderCode(VertexPath);
		std::string FragmentCode = LoadShaderCode(FragmentPath);

		CompileAndLinkShaders(VertexCode.c_str(), FragmentCode.c_str());
	}

	std::string Shader::LoadShaderCode(std::string_view ShaderPath) const
	{
		std::ifstream ShaderFile;
		ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			ShaderFile.open(ShaderPath.data());
			std::stringstream ShaderStream;
			ShaderStream << ShaderFile.rdbuf();
			ShaderFile.close();

			return ShaderStream.str();
		}
		catch (const std::ifstream::failure& f)
		{
			std::string ErrorMessage = "Error: Failed to read shader file from \'" + std::string(ShaderPath) + "\'\nException details: " + f.what();
			std::cout << ErrorMessage << std::endl;
			throw std::runtime_error(ErrorMessage);
		}
	}

	void Shader::CompileAndLinkShaders(const char* VertexCode, const char* FragmentCode)
	{
		unsigned int Vertex = CompileShader(VertexCode, GL_VERTEX_SHADER);
		unsigned int Fragment = CompileShader(FragmentCode, GL_FRAGMENT_SHADER);

		m_ID = glCreateProgram();
		glAttachShader(m_ID, Vertex);
		glAttachShader(m_ID, Fragment);
		glLinkProgram(m_ID);
		CheckCompileErrors(m_ID, "Program");

		glDeleteShader(Vertex);
		glDeleteShader(Fragment);
	}

	uint32_t Shader::CompileShader(const char* ShaderCode, GLenum ShaderType)
	{
		uint32_t Shader = glCreateShader(ShaderType);
		glShaderSource(Shader, 1, &ShaderCode, nullptr);
		glCompileShader(Shader);
		CheckCompileErrors(Shader, (ShaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment"));
		return Shader;
	}

	void Shader::Activate() const
	{
		glUseProgram(m_ID);
	}

	uint32_t Shader::GetID() const
	{
		return m_ID;
	}

	void Shader::SetBool(std::string_view Name, bool Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniform1i(UniformLocation, static_cast<int>(Value));
		}
	}

	void Shader::SetInt(std::string_view Name, int Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniform1i(UniformLocation, Value);
		}
	}

	void Shader::SetUInt(std::string_view Name, unsigned int Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniform1ui(UniformLocation, Value);
		}
	}

	void Shader::SetFloat(std::string_view Name, float Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniform1f(UniformLocation, Value);
		}
	}

	void Shader::SetDouble(std::string_view Name, double Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniform1d(UniformLocation, Value);
		}
	}

	void Shader::SetVec2(std::string_view Name, const glm::vec2& Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniform2fv(UniformLocation, 1, glm::value_ptr(Value));
		}
	}

	void Shader::SetVec2(std::string_view Name, float X, float Y) const
	{
		SetVec2(Name, glm::vec2(X, Y));
	}

	void Shader::SetVec3(std::string_view Name, const glm::vec3& Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniform3fv(UniformLocation, 1, glm::value_ptr(Value));
		}
	}

	void Shader::SetVec3(std::string_view Name, float X, float Y, float Z) const
	{
		SetVec3(Name, glm::vec3(X, Y, Z));
	}

	void Shader::SetVec4(std::string_view Name, const glm::vec4& Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniform4fv(UniformLocation, 1, glm::value_ptr(Value));
		}
	}

	void Shader::SetVec4(std::string_view Name, float X, float Y, float Z, float W) const
	{
		SetVec4(Name, glm::vec4(X, Y, Z, W));
	}

	void Shader::SetMat2(std::string_view Name, const glm::mat2& Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniformMatrix2fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
		}
	}

	void Shader::SetMat3(std::string_view Name, const glm::mat3& Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniformMatrix3fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
		}
	}

	void Shader::SetMat4(std::string_view Name, const glm::mat4& Value) const
	{
		GLint UniformLocation = GetUniformLocation(Name);
		if (UniformLocation != -1)
		{
			glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
		}
	}

	GLint Shader::GetUniformLocation(std::string_view Name) const
	{
		if (m_UniformLocationCache.find(Name.data()) != m_UniformLocationCache.end())
		{
			return m_UniformLocationCache[Name.data()];
		}
		else
		{
			GLint UniformLocation = glGetUniformLocation(m_ID, Name.data());
			m_UniformLocationCache[Name.data()] = UniformLocation;
			return UniformLocation;
		}
	}

	void Shader::CheckCompileErrors(uint32_t Shader, std::string_view Type)
	{
		int Success;
		char InfoLog[1024];
		if (Type != "Program")
		{
			glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
			if (!Success)
			{
				glGetShaderInfoLog(Shader, 1024, NULL, InfoLog);
				std::cout << "Error Shader Compilation Error of type: " << Type << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(Shader, GL_LINK_STATUS, &Success);
			if (!Success)
			{
				glGetProgramInfoLog(Shader, 1024, NULL, InfoLog);
				std::cout << "Error Program Linking: " << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

} // namespace fgl