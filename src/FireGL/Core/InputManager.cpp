#include <FireGL/Core/InputManager.h>
#include <FireGL/Core/SystemManager.h>
#include <FireGL/Core/TimeManager.h>

namespace fgl {

	void InputManager::Initialize()
	{
		RegisterWithSystemManager();
		OnInitialized();
	}

	void InputManager::RegisterWithSystemManager()
	{
		SystemManager<InputManager>::Set(this);
	}

	void InputManager::OnInitialized()
	{
	}

	void InputManager::OnMouseUpdate(double xpos, double ypos)
	{
	}

	void InputManager::OnScrollUpdate(double xoffset, double yoffset)
	{
	}

	void InputManager::OnProcessInput()
	{
	}

	void InputManager::ProcessInput()
	{
		BaseWindow* Window = SystemManager<BaseWindow>::Get();
		if (Window)
		{
			if (Window->IsFocused())
			{
				UpdateKeyState();
				OnProcessInput();
				ProcessRegisteredEvents();
			}
		}
		else
		{
			LOG_ERROR("Failed to retrieve the Window class when attempting to get the window's focus state.", true);
		}
	}

	bool InputManager::IsKeyPressed(int Key) const
	{
		auto It = m_CurrentKeyStates.find(Key);
		if (It != m_CurrentKeyStates.end())
		{
			return It->second && !m_PreviousKeyStates.at(Key);
		}
		return false;
	}

	bool InputManager::IsKeyTriggered(int Key) const
	{
		auto It = m_CurrentKeyStates.find(Key);
		if (It != m_CurrentKeyStates.end())
		{
			return It->second;
		}
		return false;
	}

	bool InputManager::IsKeyReleased(int Key) const
	{
		auto It = m_CurrentKeyStates.find(Key);
		if (It != m_CurrentKeyStates.end())
		{
			return !It->second && m_PreviousKeyStates.at(Key);
		}
		return false;
	}

	void InputManager::UpdateKeyState()
	{
		m_PreviousKeyStates = m_CurrentKeyStates;

		for (int Key = 0; Key <= GLFW_KEY_LAST; ++Key)
		{
			bool IsPressed = (glfwGetKey(SystemManager<BaseWindow>::Get()->GetWindowPtr(), Key) == GLFW_PRESS);

			// Handle key press logic
			if (IsPressed && !m_PreviousKeyStates[Key]) 
			{
				m_CurrentKeyStates[Key] = true;
			}
			// Handle key release logic
			else if (!IsPressed && m_PreviousKeyStates[Key]) 
			{
				m_CurrentKeyStates[Key] = false;
			}
		}
	}

	void InputManager::ProcessRegisteredEvents()
	{
		ProcessPressedEvents();
		ProcessTriggeredEvents();
		ProcessReleasedEvents();
	}

	void InputManager::ProcessPressedEvents()
	{
		for (const auto& [Key, Callback] : m_OnPressedCallbacks)
		{
			if (m_CurrentKeyStates[Key] && !m_PreviousKeyStates[Key])
			{
				InvokeCallback(Callback, Key, "pressed");
			}
		}
	}

	void InputManager::ProcessTriggeredEvents()
	{
		for (const auto& [Key, Callback] : m_OnTriggeredCallbacks)
		{
			if (m_CurrentKeyStates[Key])
			{
				InvokeCallback(Callback, Key, "triggered");
			}
		}
	}

	void InputManager::ProcessReleasedEvents()
	{
		for (const auto& [Key, Callback] : m_OnReleasedCallbacks)
		{
			if (!m_CurrentKeyStates[Key] && m_PreviousKeyStates[Key])
			{
				InvokeCallback(Callback, Key, "released");
			}
		}
	}

	void InputManager::InvokeCallback(const std::function<void()>& Callback, int Key, std::string_view EventType)
	{
		if (Callback)
		{
			Callback();
		}
		else
		{
			std::string Error = "Callback function for key \"" + std::string(glfwGetKeyName(Key, 0)) + "\" in the \"" + EventType.data() + "\" event was not valid.";
			LOG_ERROR(Error, true);
		}
	}

	float InputManager::GetInputDeltaTime() const
	{
		return SystemManager<TimeManager>::Get()->GetDeltaTime();
	}

	void InputManager::UpdateMouse(GLFWwindow* window, double xpos, double ypos)
	{
		OnMouseUpdate(xpos, ypos);
	}

	void InputManager::UpdateScroll(double xoffset, double yoffset)
	{
		OnScrollUpdate(xoffset, yoffset);
	}

	void InputManager::FinalizeInput()
	{
		BaseWindow* Window = SystemManager<BaseWindow>::Get();
		if (Window)
		{
			// Buffer swap and event polling take 5-10 ms, causing noticeable delay.
			glfwSwapBuffers(Window->GetWindowPtr());
			glfwPollEvents();
		}
		else
		{
			LOG_ERROR("Failed to retrieve the Window class when attempting to swap buffers and poll events.", true);
		}
	}


} // namespace fgl