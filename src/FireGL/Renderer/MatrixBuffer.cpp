#include <FireGL/Renderer/MatrixBuffer.h>

namespace fgl
{

    MatrixBuffer::MatrixBuffer()
        : m_ObjectCount(0), m_Buffer(nullptr)
    {
    }

    MatrixBuffer::MatrixBuffer(size_t ObjectCount)
        : m_ObjectCount(ObjectCount * 2) // Allocate space for twice the object count to reduce frequent resizing
    {
        m_Buffer = std::make_unique<glm::mat4[]>(m_ObjectCount * 2); // Each object may need up to 2 matrices
    }

    void MatrixBuffer::Resize(size_t NewObjectCount)
    {
        // Resize only if the requested size exceeds the current buffer capacity
        if (NewObjectCount <= m_ObjectCount)
            return;

        // Double the buffer size to minimize reallocations during instanced rendering
        m_ObjectCount = NewObjectCount * 2;
        m_Buffer = std::make_unique<glm::mat4[]>(m_ObjectCount * 2);
    }

    glm::mat4* MatrixBuffer::Get() const 
    { 
        return m_Buffer.get();
    }

    size_t MatrixBuffer::GetObjectCount() const 
    { 
        return m_ObjectCount;
    }

    size_t MatrixBuffer::GetBufferSize() const 
    { 
        return m_ObjectCount * 2 * sizeof(glm::mat4);
    }

} // namespace fgl