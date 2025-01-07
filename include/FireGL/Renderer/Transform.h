#pragma once

#include <External/glm/glm.hpp>
#include <External/glm/gtc/matrix_transform.hpp>

namespace fgl
{
    class SceneObject;
    class BaseCamera;

    /**
     * Represents the position, rotation, and scale of an object in 3D space.
     * This class handles transformations and calculates matrices for rendering in OpenGL.
     * It operates within a right-handed coordinate system by default.
     */    
    class Transform
    {
    public:
        /**
         * Constructs a Transform object with default values:
         * - Position: (0, 0, 0)
         * - Rotation: (0, 0, 0)
         * - Scale: (1, 1, 1)
         * 
         * This constructor is automatically called for any objects derived from SceneObject, 
         * ensuring that each object has a default transform.
         *
         * @param Owner The SceneObject that owns this transform.
         *              This links the transform to the parent object within the scene hierarchy.
         */
        Transform(SceneObject* Owner);

        /**
         * Sets the position of the transform in world space.
         *
         * @param X The X-coordinate of the new position.
         * @param Y The Y-coordinate of the new position.
         * @param Z The Z-coordinate of the new position.
         */
        void SetPosition(float X, float Y, float Z);

        /**
         * Sets the position of the transform using a vector.
         *
         * @param NewPosition The new position as a glm::vec3.
         */
        void SetPosition(const glm::vec3& NewPosition);

        /**
         * Moves the transform by a specified offset in world space.
         *
         * @param DeltaX Offset along the X-axis.
         * @param DeltaY Offset along the Y-axis.
         * @param DeltaZ Offset along the Z-axis.
         */
        void MoveBy(float DeltaX, float DeltaY, float DeltaZ);

        /**
         * Moves the transform by a vector offset.
         *
         * @param Offset The vector representing the offset.
         */
        void MoveBy(const glm::vec3& Offset);

        /** @return The current position as a glm::vec3. */
        const glm::vec3& GetPosition() const;

        /**
         * Sets the rotation of the transform.
         *
         * @param Pitch Rotation around the X-axis.
         * @param Yaw   Rotation around the Y-axis.
         * @param Roll  Rotation around the Z-axis.
         */
        void SetRotation(float Pitch, float Yaw, float Roll);

        /**
         * Sets the rotation using a vector.
         *
         * @param NewRotation The new rotation as a glm::vec3.
         */
        void SetRotation(const glm::vec3& NewRotation);

        /**
         * Rotates the transform by a specified offset.
         *
         * @param DeltaPitch Offset around the X-axis.
         * @param DeltaYaw   Offset around the Y-axis.
         * @param DeltaRoll  Offset around the Z-axis.
         */
        void RotateBy(float DeltaPitch, float DeltaYaw, float DeltaRoll);

        /**
         * Rotates the transform by a vector offset.
         *
         * @param RotationOffset The rotation offset as a glm::vec3.
         */
        void RotateBy(const glm::vec3& RotationOffset);

        /** @return The current rotation as a glm::vec3. */
        const glm::vec3& GetRotation() const;

        /**
         * Sets the scale of the transform.
         *
         * @param X Scale factor along the X-axis.
         * @param Y Scale factor along the Y-axis.
         * @param Z Scale factor along the Z-axis.
         */
        void SetScale(float X, float Y, float Z);

        /**
         * Sets the scale using a vector.
         *
         * @param NewScale The new scale as a glm::vec3.
         */
        void SetScale(const glm::vec3& NewScale);

        /**
         * Uniformly scales the transform.
         *
         * @param ScaleFactor The scale factor applied to all axes.
         */
        void SetUniformScale(float ScaleFactor);

        /**
         * Scales the transform incrementally along each axis.
         *
         * @param FactorX Scale factor along the X-axis.
         * @param FactorY Scale factor along the Y-axis.
         * @param FactorZ Scale factor along the Z-axis.
         */
        void ScaleBy(float FactorX, float FactorY, float FactorZ);

        /**
         * Uniformly scales the transform incrementally.
         *
         * @param Factor Scale factor applied to all axes.
         */
        void ScaleBy(const glm::vec3& ScaleOffset);

        /** @return The current scale as a glm::vec3. */
        void ScaleByUniform(float Factor);

        /** @return The current scale as a glm::vec3. */
        const glm::vec3& GetScale() const;

        /**
         * Calculates the Model-View-Projection matrix and Model matrix for rendering.
         *
         * @param OutModelViewProjection     The resulting Model-View-Projection matrix.
         * @param OutModelMatrix             The resulting Model matrix.
         */        
        void ComputeModelViewProjection(glm::mat4& OutModelViewProjection, glm::mat4& OutModelMatrix);

    private:
        /**
         * Recalculates the Model matrix based on the current position, rotation, and scale.
         * Updates the cached Model matrix and clears the dirty flag.
         */
        void RecalculateModelMatrix();

        /**
         * Applies the cached Model matrix to compute the Model-View-Projection matrix.
         *
         * @param OutModelViewProjection     The resulting Model-View-Projection matrix.
         * @param OutModelMatrix             The resulting Model matrix.
         * @param Camera                     The active camera used to retrieve the View and Projection matrices.
         */
        void ApplyCachedModelMatrix(glm::mat4& OutModelViewProjection, glm::mat4& OutModelMatrix, std::shared_ptr<BaseCamera>& Camera);

    private:
        glm::vec3 m_Position;          ///< Position in world space.
        glm::vec3 m_Rotation;          ///< Rotation in degrees (Pitch, Yaw, Roll).
        glm::vec3 m_Scale;             ///< Scale factors along each axis.

        bool m_Dirty;                  ///< Indicates whether the model matrix needs to be recalculated.
        glm::mat4 m_CachedModelMatrix; ///< Cached model matrix to avoid redundant calculations.
        SceneObject* m_Owner;          ///< The SceneObject that owns this transform.
    };

} // namespace fgl