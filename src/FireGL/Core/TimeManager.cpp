#include <FireGL/Core/TimeManager.h>
#include <FireGL/Core/SystemManager.h>

#include <External/GLFW/glfw3.h>

namespace fgl 
{

	TimeManager::TimeManager()
		: m_DeltaTime(0.f), m_LastFrame(0.f)
	{
	}

	void TimeManager::Update()
	{
		double CurrentFrame = GetTimeSeconds();
		m_DeltaTime = static_cast<float>(CurrentFrame - m_LastFrame);
		m_LastFrame = CurrentFrame;
	}

	void TimeManager::Initialize()
	{
		RegisterWithSystemManager();
		OnInitialized();
	}

	void TimeManager::OnInitialized()
	{
	}

	float TimeManager::GetDeltaTime()
	{
		return m_DeltaTime;
	}

	double TimeManager::GetTimeSeconds()
	{
		return glfwGetTime();
	}

	void TimeManager::RegisterWithSystemManager()
	{
		SystemManager<TimeManager>::Set(this);
	}

} // namespace fgl