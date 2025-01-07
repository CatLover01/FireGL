#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Core/Window.h>
#include <FireGL/Core/BaseSingletonManager.h>
#include <FireGL/Core/BaseLog.h>

namespace fgl 
{

	// Enum representing different types of key events.
	enum class KeyEventType 
	{
		OnPressed,		///< Triggered when the key is initially pressed.
		OnTriggered,	///< Triggered when the key is held down and remains active for a given duration.
		OnReleased		///< Triggered when the key is released.
	};

	/**
	 * @class InputManager
	 * @brief Manages and processes input events for various devices (keyboard, mouse, joystick).
	 *
	 * This class provides an easy-to-use input management system, where you can register and handle events for keys, mouse movements, and other inputs.
	 * You can extend its functionality to handle events like jumping or shooting by passing a player pointer during initialization and connecting input events to player actions.
	 * The class works with GLFW for keyboard inputs, and you can also use it with joysticks by referencing the appropriate keys (e.g., GLFW_KEY_...).
	 *
	 * To interact with input events, you need to ensure that this system is properly initialized and updated each frame.
	 * This class can be extended for further customization of how inputs are processed.
	 * 
	 * Example Usage:
	 * - In a derived class, you can pass a pointer to a player object in the constructor and invoke 
	 *   player-related functions (e.g., jump, move) based on the registered input events.
	 *
	 * @note For all key events, you must use GLFW key constants like GLFW_KEY_A, GLFW_KEY_B, etc., from GLFW itself.
	 */
	class InputManager : public BaseSingletonManager
	{
	public:
		/**
		 * @brief Initializes the InputManager.
		 *
		 * This function must be called after the object is created. It initializes the system and
		 * calls virtual functions that cannot be invoked during construction.
		 */
		void Initialize();

		/**
		 * @brief Processes the input events.
		 *
		 * This function must be called every frame to process the current input state. It ensures that
		 * all input events are checked, and relevant callbacks are invoked.
		 *
		 * @note If this function is not called, input events will not be processed.
		 */
		void ProcessInput();

		/**
		 * @brief Finalizes the input processing.
		 *
		 * This function should be called at the end of each frame to perform any necessary cleanup
		 * and finalize the input state, including polling events and swapping buffers.
		 */		
		void FinalizeInput();

	protected:
		/**
		 * @brief Checks if a key is pressed.
		 *
		 * This function returns `true` if the specified key is pressed in the current frame but was
		 * not pressed in the previous frame.
		 *
		 * @param Key The key to check (use GLFW_KEY_... constants from GLFW).
		 * @return `true` if the key is pressed; `false` otherwise.
		 */
		bool IsKeyPressed(int Key) const;

		/**
		  * @brief Checks if a key has been triggered.
		  *
		  * This function returns whether the specified key is currently pressed down.
		  *
		  * @param Key The key to check (use GLFW_KEY_... constants from GLFW).
		  * @return `true` if the key was triggered; `false` otherwise.
		  */
		bool IsKeyTriggered(int Key) const;

		/**
		 * @brief Checks if a key has been released.
		 *
		 * This function returns `true` if the specified key was released in the current frame.
		 *
		 * @param Key The key to check (use GLFW_KEY_... constants from GLFW).
		 * @return `true` if the key was released; `false` otherwise.
		 */
		bool IsKeyReleased(int Key) const;

		/**
		 * @brief Registers a callback function for a specific key event.
		 *
		 * @tparam Callable The type of the callback function.
		 * @tparam Args The types of the callback function's arguments.
		 * @param Key The key to register the event for (use GLFW_KEY_... constants from GLFW).
		 * @param KeyType The type of key event (OnPressed, OnTriggered, or OnReleased).
		 * @param Callback The callback function to be invoked when the event occurs.
		 * @param Args The arguments to pass to the callback function.
		 */
		template<typename Callable, typename... Args>
		void RegisterKeyEvent(int Key, KeyEventType KeyType, Callable&& Callback, Args... Arg);

		/**
		 * @brief Registers the system manager.
		 *
		 * This function ensures that the input manager is properly registered with the system manager
		 * for initialization and processing.
		 */
		void RegisterWithSystemManager() override final;
		
		/**
		 * @brief Called when the input manager is initialized.
		 *
		 * This virtual function could be overridden in a derived class to perform custom actions after
		 * initialization. It is called automatically after `Initialize`.
		 */		
		virtual void OnInitialized();

		/**
		 * @brief Called to update the mouse input.
		 *
		 * This virtual function could be overridden in a derived class to perform actions related to
		 * mouse movement. It will be called each frame when the mouse position updates.
		 *
		 * @param xpos The new mouse X position.
		 * @param ypos The new mouse Y position.
		 */
		virtual void OnMouseUpdate(double xpos, double ypos);

		/**
		 * @brief Called to update the scroll input.
		 *
		 * This virtual function could be overridden in a derived class to perform actions related to
		 * mouse scrolling. It will be called each frame when a scroll event occurs.
		 *
		 * @param xoffset The scroll offset on the X axis.
		 * @param yoffset The scroll offset on the Y axis.
		 */		
		virtual void OnScrollUpdate(double xoffset, double yoffset);

		/**
		 * @brief Processes the key input.
		 *
		 * This virtual function could be overridden in a derived class to handle custom input processing.
		 * It will be called each frame to check key states and trigger corresponding events.
		 */		
		virtual void OnProcessInput();
		
		/**
		 * @brief Retrieves the input delta time.
		 *
		 * This function returns the delta time between the current and previous frame. It is commonly
		 * used to scale input movement with time.
		 */
		float GetInputDeltaTime() const;

	public:
		/**
		 * @brief These are static functions called by the window manager class to send callbacks to this class.
		 *
		 * These functions should never be called manually, even though they are public. They are designed to be
		 * invoked by the window manager to handle mouse movement and scroll events, and should only be used in that context.
		 */
		void UpdateMouse(GLFWwindow* window, double xpos, double ypos);
		void UpdateScroll(double xoffset, double yoffset);

	private:
		/**
		 * @brief Processes registered events.
		 *
		 * This function checks all registered events and invokes the associated callback functions
		 * for the current frame.
		 */
		void ProcessRegisteredEvents();

		/**
		 * @brief Updates the key state.
		 *
		 * This function updates the state of all keys, tracking whether they are pressed or not.
		 */
		void UpdateKeyState();

		/**
		 * @brief Processes the pressed key events.
		 *
		 * This function processes all key events that are triggered when a key is pressed in 
		 * the current frame but was not pressed in the previous frame.
		 */
		void ProcessPressedEvents();

		/**
		 * @brief Processes the triggered key events.
		 *
		 * This function processes all key events that are triggered when a key is held down.
		 */
		void ProcessTriggeredEvents();

		/**
		 * @brief Processes the released key events.
		 *
		 * This function processes all key events that are triggered when a key is released.
		 */
		void ProcessReleasedEvents();

		/**
		 * @brief Invokes the callback function for a specific key event.
		 *
		 * This function invokes the callback function associated with a key event, passing the key and
		 * event type as arguments.
		 *
		 * @param Callback The callback function to invoke.
		 * @param Key The key associated with the event (Debug only).
		 * @param EventType The type of the event (pressed, triggered, or released) (Debug only).
		 */
		void InvokeCallback(const std::function<void()>& Callback, int Key, std::string_view EventType);

	private:
		// State tracking for the key states (pressed, released, etc.)
		std::unordered_map<int, bool> m_CurrentKeyStates;
		std::unordered_map<int, bool> m_PreviousKeyStates;

		// Callbacks for the key events
		std::unordered_map<int, std::function<void()>> m_OnPressedCallbacks;
		std::unordered_map<int, std::function<void()>> m_OnTriggeredCallbacks;
		std::unordered_map<int, std::function<void()>> m_OnReleasedCallbacks;
	};

	/**
	 * @brief Registers a key event callback to be triggered on specific key actions.
	 *
	 * Note:
	 * - Any return values from the provided callback will be ignored.
	 * - The arguments provided must match the parameters of the callback function. If not, a compile-time error will occur.
	 * - For member functions, you must pass the class instance as an argument (e.g., `RegisterKeyEvent(&DerivedClass::foo, this, 1)`).
	 */
	template<typename Callable, typename... Args>
	void InputManager::RegisterKeyEvent(int Key, KeyEventType KeyType, Callable&& Callback, Args... Arg)
	{
		auto WrappedCallback = [callback = std::forward<Callable>(Callback),
			...args = std::forward<Args>(Arg)]() mutable {
			std::invoke(callback, std::forward<Args>(args)...);
		};

		switch (KeyType)
		{
		case KeyEventType::OnPressed:
		{
			m_OnPressedCallbacks[Key] = WrappedCallback;
			break;
		}
		case KeyEventType::OnTriggered:
		{
			m_OnTriggeredCallbacks[Key] = WrappedCallback;
			break;
		}
		case KeyEventType::OnReleased:
		{
			m_OnReleasedCallbacks[Key] = WrappedCallback;
			break;
		}
		default:
		{
			LOG_ERROR("Unexpected KeyEventType provided", true);
		}
		}
	}

} // namespace fgl