#include <FireGL/Renderer/Transform.h>
#include <FireGL/Renderer/SceneObject.h>
#include <FireGL/Renderer/BaseCamera.h>
#include <FireGL/Core/BaseLog.h>

namespace fgl
{
    Transform::Transform(SceneObject* Owner)
        : m_Position(0.0f), m_Rotation(0.0f), 
          m_Scale(1.0f), m_Owner(Owner),
          m_Dirty(true)
    {
    }

    void Transform::SetPosition(float X, float Y, float Z)
    {
        m_Position = glm::vec3(X, Y, Z);
        m_Dirty = true;
    }

    void Transform::SetPosition(const glm::vec3& NewPosition)
    {
        m_Position = NewPosition;
        m_Dirty = true;
    }

    void Transform::MoveBy(float DeltaX, float DeltaY, float DeltaZ)
    {
        m_Position += glm::vec3(DeltaX, DeltaY, DeltaZ);
        m_Dirty = true;
    }

    void Transform::MoveBy(const glm::vec3& Offset)
    {
        m_Position += Offset;
        m_Dirty = true;
    }

    const glm::vec3& Transform::GetPosition() const
    {
        return m_Position;
    }

    void Transform::SetRotation(float Pitch, float Yaw, float Roll)
    {
        m_Rotation = glm::vec3(Pitch, Yaw, Roll);
        m_Dirty = true;
    }

    void Transform::SetRotation(const glm::vec3& NewRotation)
    {
        m_Rotation = NewRotation;
        m_Dirty = true;
    }

    void Transform::RotateBy(float DeltaPitch, float DeltaYaw, float DeltaRoll)
    {
        m_Rotation += glm::vec3(DeltaPitch, DeltaYaw, DeltaRoll);
        m_Dirty = true;
    }

    void Transform::RotateBy(const glm::vec3& RotationOffset)
    {
        m_Rotation += RotationOffset;
        m_Dirty = true;
    }

    const glm::vec3& Transform::GetRotation() const
    {
        return m_Rotation;
    }

    void Transform::SetScale(float X, float Y, float Z)
    {
        m_Scale = glm::vec3(X, Y, Z);
        m_Dirty = true;
    }

    void Transform::SetScale(const glm::vec3& NewScale)
    {
        m_Scale = NewScale;
        m_Dirty = true;
    }

    void Transform::SetUniformScale(float ScaleFactor)
    {
        m_Scale = glm::vec3(ScaleFactor);
        m_Dirty = true;
    }

    void Transform::ScaleBy(float FactorX, float FactorY, float FactorZ)
    {
        m_Scale *= glm::vec3(FactorX, FactorY, FactorZ);
        m_Dirty = true;
    }

    void Transform::ScaleBy(const glm::vec3& ScaleOffset)
    {
        m_Scale *= ScaleOffset;
        m_Dirty = true;
    }

    void Transform::ScaleByUniform(float Factor)
    {
        m_Scale *= Factor;
        m_Dirty = true;
    }

    const glm::vec3& Transform::GetScale() const
    {
        return m_Scale;
    }

    void Transform::ComputeModelViewProjection(glm::mat4& OutModelViewProjection, glm::mat4& OutModelMatrix)
    {
        if (!m_Owner)
        {
            LOG_ERROR("Invalid owner. For camera usage, call the appropriate function to retrieve the MVP.", true)
        }
        std::shared_ptr<BaseCamera> ActiveCamera = m_Owner->GetScene()->GetActiveCamera();

        if (m_Dirty)
        {
            RecalculateModelMatrix();
        }

        ApplyCachedModelMatrix(OutModelViewProjection, OutModelMatrix, ActiveCamera);
    }

    void Transform::RecalculateModelMatrix()
    {
        m_CachedModelMatrix =
            glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1, 0, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1)) *
            glm::scale(glm::mat4(1.0f), m_Scale);

        m_Dirty = false;
    }

    void Transform::ApplyCachedModelMatrix(glm::mat4& OutModelViewProjection, glm::mat4& OutModelMatrix, std::shared_ptr<BaseCamera>& Camera)
    {
        OutModelMatrix = m_CachedModelMatrix;
        OutModelViewProjection = Camera->GetProjectionMatrix() * Camera->GetViewMatrix() * m_CachedModelMatrix;
    }

} // namespace fgl