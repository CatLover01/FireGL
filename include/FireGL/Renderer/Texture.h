#pragma once

#include <FireGL/fglpch.h>

#include <External/glad/glad.h>

#include <cstdint>

namespace fgl
{

    /**
     * Represents a texture in OpenGL, supporting both 2D textures and CubeMaps.
     * This class manages texture loading, binding, and resource cleanup for textures used in rendering.
     * It handles both regular 2D textures as well as CubeMap textures, such as those used for skyboxes.
     */
    class Texture 
    {
    public:
        /**
         * Constructs a Texture object with an uninitialized ID.
         * The texture is not loaded or bound at this point.
         */
        Texture();

        /**
         * Loads a 2D texture from a specified file path and sets OpenGL texture parameters.
         * The texture is generated, bound, and initialized with the data loaded from the file.
         *
         * @param Path           The path to the texture file (can be relative or absolute).
         * @param WrapS          The wrapping mode for the horizontal axis (U-coordinate).
         * @param WrapT          The wrapping mode for the vertical axis (V-coordinate).
         * @param MinFilter      The minification filter used when the texture is scaled down.
         * @param MagFilter      The magnification filter used when the texture is scaled up.
         * @param FlipVertical   Whether or not to flip the texture vertically upon loading.
         * @return               `true` if the texture was successfully loaded, `false` otherwise.
         */
        bool LoadTexture(
            std::string_view Path, 
            GLenum WrapS = GL_REPEAT,
            GLenum WrapT = GL_REPEAT,
            GLenum MinFilter = GL_LINEAR,
            GLenum MagFilter = GL_LINEAR,
            bool FlipVertical = true
        );

        /**
         * Loads a CubeMap texture from multiple faces and sets OpenGL texture parameters.
         * The CubeMap is created by loading six images (one for each face) and binding them as a CubeMap texture.
         *
         * The six faces should be provided in the following order:
         * - +X (right face)
         * - -X (left face)
         * - +Y (top face)
         * - -Y (bottom face)
         * - +Z (front face)
         * - -Z (back face)
         *
         * @param PathToFaces    A list of paths to the six faces of the CubeMap in the order described above.
         * @param MinFilter      The minification filter used when the CubeMap is scaled down.
         * @param MagFilter      The magnification filter used when the CubeMap is scaled up.
         * @param FlipVertical   Whether or not to flip the texture vertically upon loading.
         * @return               `true` if the CubeMap was successfully loaded, `false` otherwise.
         */
        bool LoadCubeMap(
            const std::vector<std::string>& PathToFaces,
            GLenum MinFilter = GL_LINEAR,
            GLenum MagFilter = GL_LINEAR,
            bool FlipVertical = false
        );

        /**
         * Binds the texture for use in rendering.
         * This activates the texture and makes it available to OpenGL for rendering.
         */
        void Activate() const;

        /**
         * Cleans up the OpenGL resources associated with the texture.
         * This deletes the texture object from OpenGL to free the resources.
         */
        void Cleanup() const;


        /// Getters

        /** @return The OpenGL texture ID. */
        unsigned int GetID() const;

        /** @return The name of the texture (e.g., "diffuse", "specular", etc.). */
        std::string GetName() const;

        /** @return The file path from which the texture was loaded. */
        std::string GetPath() const;

        /** @return The slot index for the texture, representing the binding point in OpenGL. */
        int8_t GetSlotIndex() const;


        /// Setters

        /** Sets the OpenGL texture ID for the texture. */
        void SetID(unsigned int ID);

        /** Sets the name of the texture (e.g., "diffuse", "specular", etc.). */
        void SetName(std::string_view Name);

        /** Sets the file path from which the texture was loaded. */
        void SetPath(std::string_view Path);

        /** Sets the slot index for the texture, representing the binding point in OpenGL. */
        void SetSlotIndex(int8_t SlotIndex);

    private:
        /**
         * Configures the texture's wrapping and filtering parameters based on the provided values.
         *
         * See the LoadTexture function params for detailed descriptions of each parameter.
         */
        void SetupTextureParameters(GLenum WrapS, GLenum WrapT, GLenum MinFilter, GLenum MagFilter);

        /**
         * Configures CubeMap specific parameters, such as minification and magnification filters.
         */
        void SetupCubeMapParameters(GLenum MinFilter, GLenum MagFilter);

        /**
         * Loads texture data from a specified file path using the stb_image library and uploads it to OpenGL.
         * This function can load both 2D textures and CubeMap faces depending on the specified target.
         *
         * @param Path   The path to the texture file. The file can represent either a 2D texture or a CubeMap face.
         * @param Target The OpenGL texture target (default is `GL_TEXTURE_2D`, but can also be `GL_TEXTURE_CUBE_MAP` for CubeMap textures).
         * @return `true` if the texture data was successfully loaded and uploaded to OpenGL, `false` otherwise.
         */
        bool LoadTextureFromFile(std::string_view Path, GLenum Target = GL_TEXTURE_2D);

        /**
         * Handles the case when texture loading fails.
         * Logs an error and performs necessary clean-up operations.
         */
        void HandleTextureLoadingFailure();

    private:
        unsigned int m_ID;       ///< The OpenGL texture ID assigned after texture creation
        std::string m_Name;      ///< The type of texture (e.g., diffuse, specular, roughness) (not needed except if it's created in the material class)
        std::string m_Path;      ///< The file path from which the texture was loaded 
        int8_t m_SlotIndex;      ///< The texture slot index (binds the texture to a particular active texture unit)
        GLenum m_TextureTarget;  ///< The OpenGL texture target (2D texture or CubeMap)
    };

} // namespace fgl