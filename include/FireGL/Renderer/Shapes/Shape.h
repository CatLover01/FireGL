#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Renderer/Mesh.h>
#include <FireGL/Renderer/SceneObject.h>

namespace fgl
{
    class Material;
    class Texture;
    class Vertex;

    /**
     * Represents a predefined shape in the scene with static properties and potential physics support (to be added in the future).
     * This class serves as the main entry point for predefined shapes, handling basic optimizations
     * and grouping logic. The goal is to provide simple, static shapes that are useful primarily for
     * testing purposes or in cases where custom logic is unnecessary.
     *
     * Note: This class is designed specifically for static predefined shapes and is not intended to
     * handle complex scene dynamics or custom logic. For dynamic or more complex objects, consider
     * using different classes that extend the SceneObject base class(e.g. Entity).
     * 
     * This class is meant to be derived from. Cube and Sphere shapes are provided by default, 
     * but you can add your own, such as Cones or Cylinders, as needed. These basic shapes can also serve as 
     * excellent collision primitives since fewer vertices/points typically improve performance during collision checks.
     */
	class Shape : public SceneObject
	{
    public:
        /**
         * Constructs a Shape object using the provided vertex, and index.
         * This constructor creates a static predefined shape that can be used for rendering or testing.
         *
         * @param Vertices  A vector containing the vertices of the shape.
         * @param Indices   A vector containing the indices for the shape's mesh.
         */
        Shape(std::vector<Vertex>&& Vertices, std::vector<unsigned int>&& Indices);
        
        /** Virtual destructor to allow proper cleanup of derived classes. */
        virtual ~Shape() = default;

        /**
         * Sets the material for the shape.
         *
         * @param Material  The material to apply to the shape.
         */
        virtual void SetMaterial(std::shared_ptr<Material> Material) override;

        /**
         * Returns the material applied to the shape.
         * MeshIndex is ignored in this case.
         *
         * @return A shared pointer to the material.
         */
        virtual const std::shared_ptr<Material> GetMaterial(size_t MeshIndex = 0) const override;

        /**
         * @brief Provides access to the meshes that make up the shape.
         *
         * @return A reference to a vector of BaseMesh objects.
         */
        virtual std::vector<BaseMesh>& GetMeshes() override;

        /**
         * @brief Retrieves the hash representing the shape.
         *
         * @return The unique hash of the shape, generated during initialization.
         */
        virtual size_t GetHash() const override;

        /**
         * Renders the shape in the scene.
         * This function overrides the `Render` method from the SceneObject base class and is
         * responsible for rendering the shape using its mesh and material.
         */
        virtual void Render(size_t NumberInstance) const override;

    private:
        /**
         * These three functions are required to be derived from SceneObject but are not needed here.
         * They are left empty to prevent implementation, as Shape is intended for static objects.
         */
        virtual void Tick(float DeltaTime) override final;
        virtual void BeginPlay() override final;
        virtual void Destroy() override final;

    private:
        /**
         * @brief A single mesh representing the shape.
         *
         * @details This vector will always contain exactly one mesh, representing the shape's static geometry.
         *          Although the vector structure allows for compatibility with classes handling multiple meshes
         *          (such as Model), Shape inherently uses only a single entry.
         */
        std::vector<BaseMesh> m_Mesh;
	};

} // namespace fgl