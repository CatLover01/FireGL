#include <FireGL/Renderer/Texture.h>
#include <FireGL/Core/BaseLog.h>

#include <External/stb/stb_image.h>

namespace fgl
{

    Texture::Texture()
        : m_ID(0), m_SlotIndex(0)
    {
    }

    void Texture::Activate() const
    {
        glActiveTexture(GL_TEXTURE0 + m_SlotIndex);
        glBindTexture(m_TextureTarget, m_ID);
    }

    void Texture::Cleanup() const
    {
        glDeleteTextures(1, &m_ID);
    }

    bool Texture::LoadTexture(std::string_view Path, GLenum WrapS, GLenum WrapT, GLenum MinFilter, GLenum MagFilter, bool FlipVertical)
    {
        m_TextureTarget = GL_TEXTURE_2D;
        m_Path = Path;

        stbi_set_flip_vertically_on_load(FlipVertical);
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        if (!LoadTextureFromFile(Path))
        {
            HandleTextureLoadingFailure();
            return false;
        }

        SetupTextureParameters(WrapS, WrapT, MinFilter, MagFilter);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }

    bool Texture::LoadCubeMap(const std::vector<std::string>& PathToFaces, GLenum MinFilter, GLenum MagFilter, bool FlipVertical)
    {
        if (PathToFaces.size() != 6) 
        {
            LOG_ERROR("CubeMap loading failed: Expected 6 faces, but received " + std::to_string(PathToFaces.size()) + ".", true)
            return false;
        }
        m_Path = PathToFaces[0];  // Just for logging purposes
        m_TextureTarget = GL_TEXTURE_CUBE_MAP;

        stbi_set_flip_vertically_on_load(FlipVertical);
        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);


        for (int i = 0; i < PathToFaces.size(); ++i)
        {
            if (!LoadTextureFromFile(PathToFaces[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i))
            {
                HandleTextureLoadingFailure();
                return false;
            }
        }
        
        SetupCubeMapParameters(MinFilter, MagFilter);
        return true;
    }

    void Texture::SetupTextureParameters(GLenum WrapS, GLenum WrapT, GLenum MinFilter, GLenum MagFilter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter);
    }

    void Texture::SetupCubeMapParameters(GLenum MinFilter, GLenum MagFilter)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, MinFilter);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, MagFilter);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    bool Texture::LoadTextureFromFile(std::string_view Path, GLenum Target)
    {
        int Width, Height, ChannelNr;
        unsigned char* Data = stbi_load(Path.data(), &Width, &Height, &ChannelNr, 0);
        if (Data)
        {
            GLenum Format = (ChannelNr == 3) ? GL_RGB : GL_RGBA;
            if (Target == GL_TEXTURE_2D)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
            }
            else
            {
                glTexImage2D(Target, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
            }
            stbi_image_free(Data);
            return true;
        }
        else
        {
            stbi_image_free(Data);
            return false;
        }
    }

    void Texture::HandleTextureLoadingFailure()
    {
        LOG_ERROR("Failed to load texture at path: " + std::string(m_Path), false);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    unsigned int Texture::GetID() const 
    {
        return m_ID;
    }

    std::string Texture::GetName() const 
    {
        return m_Name;
    }

    std::string Texture::GetPath() const 
    {
        return m_Path;
    }

    int8_t Texture::GetSlotIndex() const 
    {
        return m_SlotIndex;
    }

    void Texture::SetID(unsigned int ID)
    {
        m_ID = ID;
    }

    void Texture::SetName(std::string_view Name)
    {
        m_Name = Name;
    }

    void Texture::SetPath(std::string_view Path)
    {
        m_Path = Path;
    }

    void Texture::SetSlotIndex(int8_t SlotIndex)
    {
        m_SlotIndex = SlotIndex;
    }

} // namespace fgl