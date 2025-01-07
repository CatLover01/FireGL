#include <FireGL/Renderer/Shapes/Shape.h>
#include <FireGL/Renderer/Texture.h>
#include <FireGL/Renderer/Vertex.h>
#include <FireGL/Renderer/Material.h>

namespace fgl
{

	Shape::Shape(std::vector<Vertex>&& Vertices, std::vector<unsigned int>&& Indices)
		: m_Mesh{ { std::move(Vertices), std::move(Indices), {}, 1 } }, SceneObject()
	{
	}

	std::vector<BaseMesh>& Shape::GetMeshes()
	{
		return { m_Mesh };
	}

	size_t Shape::GetHash() const
	{
		return m_Mesh[0].GetMeshHash();
	}

	void Shape::Render(size_t NumberInstance) const
	{
		m_Mesh[0].Render(NumberInstance);
	}

	void Shape::SetMaterial(std::shared_ptr<Material> Material)
	{
		m_Mesh[0].SetMaterial(Material);
		Material->SetSceneObject(this);
	}

	const std::shared_ptr<Material> Shape::GetMaterial(size_t MeshIndex) const
	{
		return m_Mesh[0].GetMaterial();
	}

	void Shape::Tick(float DeltaTime)
	{
	}

	void Shape::BeginPlay()
	{
	}

	void Shape::Destroy()
	{
	}

} // namespace fgl