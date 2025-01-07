#include <FireGL/Renderer/Scene.h>
#include <FireGL/Core/TimeManager.h>
#include <FireGL/Core/SystemManager.h>
#include <FireGL/Renderer/SceneObject.h>
#include <FireGL/Renderer/BaseCamera.h>

namespace fgl
{

	Scene::Scene()
		: m_ActiveCamera(nullptr)
	{
	}
	Scene::Scene(std::shared_ptr<BaseCamera> ActiveCamera)
		: m_ActiveCamera(ActiveCamera)
	{
		AddCamera(ActiveCamera);
	}

	void Scene::AddObject(std::unique_ptr<SceneObject> Object)
	{
		Object->SetScene(this);
		Object->BeginPlay();
		m_Objects.push_back(std::move(Object));
	}

	const std::vector<std::unique_ptr<SceneObject>>& Scene::GetObjects() const
	{
		return m_Objects;
	}

	void Scene::Process()
	{
		m_ActiveCamera->UpdateViewMatrix();

		for (const auto& Object : m_Objects)
		{
			TimeManager* Manager = SystemManager<TimeManager>::Get();
			if (Manager)
			{
				Object->Tick(Manager->GetDeltaTime());
			}
		}
	}

	void Scene::SetActiveCamera(std::shared_ptr<BaseCamera> ActiveCamera)
	{
		m_ActiveCamera = ActiveCamera;
	}

	std::shared_ptr<BaseCamera> Scene::GetActiveCamera() const
	{
		return m_ActiveCamera;
	}

	void Scene::AddCamera(std::shared_ptr<BaseCamera> Camera)
	{
		m_Cameras.push_back(Camera);
	}

	void Scene::RemoveCamera(std::shared_ptr<BaseCamera> Camera)
	{
		m_Cameras.erase(std::remove(m_Cameras.begin(), m_Cameras.end(), Camera), m_Cameras.end());
	}

} // namespace fgl