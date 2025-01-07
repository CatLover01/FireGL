#include <FireGL/Renderer/SceneObject.h>

namespace fgl
{

	SceneObject::SceneObject()
		: m_Transform(this),
		  m_OwningScene(nullptr),
		  m_New(true)
	{
	}

	void SceneObject::SetScene(Scene* Scene) 
	{ 
		m_OwningScene = Scene; 
	}
	
	const Scene* const SceneObject::GetScene() const 
	{ 
		return m_OwningScene; 
	}

	Transform& SceneObject::GetTransform()
	{
		return m_Transform;
	}

	const Transform& SceneObject::GetTransform() const
	{
		return m_Transform;
	}

	bool SceneObject::IsNew() const
	{
		return m_New;
	}

	void SceneObject::SetNew(bool IsNew)
	{
		m_New = IsNew;
	}

} // namespace fgl