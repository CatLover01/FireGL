#include <FireGL/Renderer/Entity.h>
#include <FireGL/Renderer/Component.h>
#include <FireGL/Renderer/Mesh.h>
#include <FireGL/Renderer/Transform.h>

namespace fgl
{
	std::vector<BaseMesh>& Entity::GetMeshes()
	{
		return m_Object->GetMeshes();
	}
	size_t Entity::GetHash() const
	{
		return m_Object->GetHash();
	}
	
	void Entity::SetMaterial(std::shared_ptr<Material> Material)
	{
		m_Object->SetMaterial(Material);
	}

	const std::shared_ptr<Material> Entity::GetMaterial(size_t MeshIndex) const
	{
		return m_Object->GetMaterial();
	}

	void Entity::Render(size_t NumberInstance) const
	{
		OnPrepareRender();
		m_Object->Render(NumberInstance);
		OnPostRender();
	}

	void Entity::Tick(float DeltaTime)
	{
		for (const auto& [Key, Component] : m_Components)
		{
			Component->OnTick(DeltaTime);
		}

		m_Object->Tick(DeltaTime);
		OnTick(DeltaTime);
	};

	void Entity::BeginPlay()
	{
		for (const auto& [Key, Component] : m_Components)
		{
			Component->OnBeginPlay();
		}

		m_Object->BeginPlay();
		OnBeginPlay();
	}

	void Entity::Destroy()
	{
		for (const auto& [Key, Component] : m_Components)
		{
			Component->OnDestroyed();
		}

		OnDestroyed();
		m_Object->Destroy();
	}

	void Entity::OnTick(float DeltaTime)
	{
	}

	void Entity::OnBeginPlay()
	{
	}

	void Entity::OnDestroyed()
	{
	}

	void Entity::OnPrepareRender() const
	{
	}

	void Entity::OnPostRender() const
	{
	}

	void Entity::RemoveComponent(size_t ID)
	{
		m_Components.erase(ID);
	}

} // namespace fgl