#pragma once

#include <FireGL/fglpch.h>

#include <External/glm/mat4x4.hpp>

namespace fgl
{

    /**
     * MatrixBuffer class responsible for managing a dynamic array of transformation matrices.
     *
     * This class handles memory allocation for a buffer of glm::mat4 matrices, typically used
     * for instanced rendering. It stores object transformations and can dynamically resize
     * to accommodate a variable number of instances, minimizing reallocation frequency.
     */
    class MatrixBuffer
    {
    public:
        /**
         * Default constructor for MatrixBuffer.
         * Initializes an empty buffer with no allocated objects.
         */
        MatrixBuffer();

        /**
         * Constructs a MatrixBuffer with an initial capacity for a given number of objects.
         *
         * @param ObjectCount The initial number of objects to allocate space for.
         *        Internally allocates space for double the provided count to reduce future allocations.
         */        
        MatrixBuffer(size_t ObjectCount);

        /**
         * Resizes the matrix buffer to accommodate a new number of objects.
         *
         * If the new size is larger than the current capacity, the buffer is reallocated
         * to accommodate twice the new object count to minimize frequent reallocations.
         *
         * @param NewObjectCount The new number of objects to allocate space for.
         */
        void Resize(size_t NewObjectCount);

        /**
         * Retrieves a pointer to the buffer containing glm::mat4 matrices.
         *
         * @return Pointer to the matrix buffer.
         */
        glm::mat4* Get() const;

        /**
         * Returns the current number of objects that can fit in the buffer.
         *
         * @return The total object count the buffer can hold.
         */
        size_t GetObjectCount() const;

        /**
         * Returns the total size (in bytes) of the allocated buffer.
         *
         * @return Size of the buffer in bytes.
         */
        size_t GetBufferSize() const;

    private:
        std::unique_ptr<glm::mat4[]> m_Buffer; ///< Pointer to the dynamically allocated buffer of matrices.
        
        /**
         * Number of objects that can currently fit in the buffer.
         *
         * Each object requires 2 glm::mat4 matrices, so this count reflects the total
         * number of objects, not individual matrices.
         */        
        size_t m_ObjectCount;
    };

} // namespace fgl