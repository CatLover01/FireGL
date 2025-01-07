#include <FireGL/Renderer/Component.h>
#include <FireGL/Renderer/Entity.h>

namespace fgl
{

    void Component::SetOwner(Entity* Owner)
    {
        m_Owner = Owner;
    }

    Entity* Component::GetOwner() const
    {
        return m_Owner;
    }

    void Component::OnBeginPlay()
    {
    }

    void Component::OnTick(float DeltaTime)
    {
    }

    void Component::OnDestroyed()
    {
    }

} // namespace fgl