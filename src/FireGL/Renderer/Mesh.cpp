#include <FireGL/Renderer/Mesh.h>
#include <FireGL/Renderer/Material.h>

namespace fgl
{

    BaseMesh::BaseMesh(std::vector<Vertex>&& Vertices, std::vector<unsigned int>&& Indices, std::vector<Texture>&& Textures, bool bComputeHash)
		:
		m_Vertices( std::move(Vertices) ),
		m_Indices( std::move(Indices) ),
		m_Textures( std::move(Textures) )
	{
        if (bComputeHash)
        {
            ComputeHash();
        }
	}

    void BaseMesh::FirstPass()
    {
        InitializeBuffers();
        ConfigureVertexAttributes();
    }
    
    void BaseMesh::SecondPass()
    {
        ConfigureVertexAttributesInstances();
    }

	void BaseMesh::InitializeBuffers()
	{
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);
	}

    void BaseMesh::ConfigureVertexAttributesInstances()
    {
        glBindVertexArray(VAO);

        std::size_t vec4Size = sizeof(glm::vec4);

        // Configure the MVP matrix for instancing
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(1 * vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(3 * vec4Size));

        // Configure the Model matrix for instancing
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(4 * vec4Size));
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(5 * vec4Size));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(6 * vec4Size));
        glEnableVertexAttribArray(10);
        glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 8 * vec4Size, (void*)(7 * vec4Size));

        // Set the divisor to 1 for instancing (this updates per instance)
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);
        glVertexAttribDivisor(10, 1);

        glBindVertexArray(0);
    }

    void BaseMesh::ConfigureVertexAttributes()
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }

    void BaseMesh::Render(size_t NumberInstance) const
    {
        if (m_Material)
        {
            m_Material->Activate();
        }

        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, (GLvoid*)(0), NumberInstance);
        glBindVertexArray(0);
    }

    void BaseMesh::ComputeHash()
    {
        // Compute a hash for the vertices and indices to batch meshes for instancing
        size_t hash = std::hash<size_t>()(m_Vertices.size());

        // Hash the size and content of the vertices
        for (const Vertex& vertex : m_Vertices) {
            hash ^= std::hash<float>()(vertex.Position.x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<float>()(vertex.Position.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<float>()(vertex.Position.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }

        // Hash the indices to ensure unique mesh identification
        for (const uint32_t& index : m_Indices) {
            hash ^= std::hash<uint32_t>()(index) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }

        m_MeshHash = hash;
    }

    size_t BaseMesh::GetMeshHash() const
    {
        return m_MeshHash;
    }

    void BaseMesh::SetMaterial(std::shared_ptr<Material> Material)
    {
        m_Material = Material;
    }

    const std::shared_ptr<Material> BaseMesh::GetMaterial() const
    {
        return m_Material;
    }

    std::vector<Texture>& BaseMesh::GetTextures()
    { 
        return m_Textures; 
    }

} // namespace fgl