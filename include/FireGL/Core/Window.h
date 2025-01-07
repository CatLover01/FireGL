#pragma once

#include <FireGL/fglpch.h>
#include <FireGL/Core/BaseSingletonManager.h>

#include <External/glad/glad.h>
#include <External/GLFW/glfw3.h>

namespace fgl 
{
	class InputManager;

	// Represents the different types of windows that can be created.
	enum class WindowType
	{
		FullScreen,			  ///< Fullscreen without borders.
		WindowedFullScreen,   ///< Fullscreen with borders (maximized window).
		Windowed,			  ///< Windowed mode with a custom size.
		BorderlessWindowed    ///< Borderless window mode (for custom border implementations).
	};

	/**
	 * @class BaseWindow
	 * @brief Manages a GLFW window and its associated settings.
	 *
	 * This class provides functionality to create and manage a window with an OpenGL context
	 * using GLFW. It handles tasks such as window initialization, resizing, managing focus states,
	 * and optionally enabling VSync.
	 *
	 * @note Only one instance of this class is allowed at a time. Creating multiple instances
	 *       can lead to unpredictable behavior. The class assumes that only a single window
	 *       will be managed, and the instance must be properly initialized before rendering begins.
	 * @note Window attributes and window hints are not implemented in this class due to their specificity.
	 *		 For detailed usage, refer to the official GLFW documentation. If needed, you can extend
	 *       this class to implement custom window attributes and functionalities, as this class
	 *       serves as a foundation to build upon.
	 */
	class BaseWindow : public BaseSingletonManager
	{
	public:
		/**
		 * @brief Initializes the window with the specified parameters.
		 *
		 * This function creates and initializes a GLFW window. It must be called early in the application's lifecycle, as many other components depend on a valid window instance.
		 *
		 * @param WindowType           Specifies the type of window (e.g., fullscreen, windowed, borderless windowed).
		 * @param WindowWidth          Optional fixed window width (applied only in non-fullscreen modes).
		 * @param WindowHeight         Optional fixed window height (applied only in non-fullscreen modes).
		 *
		 * @note
		 * - This function **must** be the first one called when working with the window,
		 *		as calling other window-related functions before it completes will result in errors.
		 *
		 * @throws std::runtime_error if GLFW initialization or window creation fails.
		 */
		void Initialize(
			int OpenGLMajorVersion,
			int OpenGLMinorVersion,
			std::string_view ApplicationName,
			WindowType WindowType,
			bool VSyncEnabled = true,
			std::optional<int> WindowWidth = std::optional<int>{ std::nullopt },
			std::optional<int> WindowHeight = std::optional<int>{ std::nullopt }
		);


		/**
		 * @brief Retrieves the GLFW window pointer.
		 *
		 * Provides access to the underlying `GLFWwindow` instance used by the application.
		 * Useful for advanced operations that require direct GLFW API calls..
		 */
		[[nodiscard]] GLFWwindow* GetWindowPtr() const;

		/**
		 * @brief Determines if the window is scheduled to close.
		 *
		 * Checks the internal GLFW state to decide if the window is marked for closure.
		 * Commonly used in the main application loop to determine when to exit.
		 *
		 * @return True if the window should close, false otherwise.
		 */
		[[nodiscard]] bool ShouldClose() const;

		/**
		 * @brief Checks if the window is currently in focus.
		 *
		 * Determines if the window has focus, indicating whether it is the active window
		 * receiving user input and interactions.
		 *
		 * @return True if the window is focused, false otherwise.
		 */
		[[nodiscard]] bool IsFocused() const;

		/**
		 * @brief Gets the current window title.
		 *
		 * Retrieves the title text displayed on the window's title bar.
		 */
		[[nodiscard]] const char* GetWindowTitle() const;

		/**
		 * @brief Retrieves the window's current opacity level.
		 *
		 * The opacity determines how transparent the window appears, with values ranging
		 * from 0 (fully transparent) to 1 (fully opaque).
		 */
		[[nodiscard]] float GetWindowOpacity() const;

		/**
		 * @brief Retrieves the window's current aspect ratio.
		 *
		 * The aspect ratio is the ratio of the window's width to its height.
		 */
		[[nodiscard]] float GetAspectRatio() const;

		/**
		 * @brief Retrieves the current size of the window.
		 *
		 * Updates the provided `Width` and `Height` references with the window's current size in screen coordinates.
		 */
		void GetWindowSize(int& Width, int& Height) const;

		/**
		 * @brief Gets the cursor's position relative to the window.
		 *
		 * Returns the current X and Y coordinates of the cursor within the window. The
		 * position is relative to the top-left corner of the window.
		 */
		void GetCursorPosition(double& XPos, double& YPos) const;


		/**
		 * @brief Enables or disables vertical synchronization (VSync).
		 */
		void SetVSync(bool VSyncEnabled);

		/**
		 * @brief Updates the window's title.
		 *
		 * Use this function to dynamically change the window's title after creation.
		 */
		void SetWindowTitle(std::string_view Title);

		/**
		 * @brief Sets the window's opacity.
		 *
		 * Adjusts the transparency of the window.
		 * Valid values range from `0.0` (fully transparent) to `1.0` (fully opaque).
		 */
		void SetWindowOpacity(float Opacity);

		/**
		 * @brief Sets the position of the window on the screen.
		 *
		 * Positions the window relative to the upper-left corner of the primary monitor.
		 * @param WidthFromLeft Distance in pixels from the left edge of the screen.
		 * @param HeightFromTop Distance in pixels from the top edge of the screen.
		 */
		void SetWindowPosition(int WidthFromLeft, int HeightFromTop);

		/**
		 * @brief Centers the window on the screen.
		 *
		 * Automatically calculates and sets the window position so it is centered
		 * on the primary monitor.
		 */
		void SetWindowPositionCenter();

		/**
		 * @brief Restricts the window's aspect ratio to a fixed ratio.
		 *
		 * This function ensures the window can only be resized while maintaining the specified
		 * aspect ratio (e.g., 16:9 for widescreen). Useful for applications where maintaining
		 * a consistent display ratio is critical, such as games or video playback.
		 */
		void SetWindowAspectRatio(int WidthRatio, int HeightRatio);

		/**
		 * @brief Sets the window's icon from an image file.
		 *
		 * Loads an image from the specified file path and sets it as the window's icon.
		 * This function uses the STB image library to load the image in RGBA format and
		 * applies it to the window through GLFW. The icon is automatically freed from memory
		 * after being set. If loading fails, an error is logged to the console.
		 *
		 * @param IconPath Path to the image file to use as the window icon (supports PNG, JPG, etc.).
		 */
		void SetWindowIcon(std::string_view IconPath);

		/**
		 * @brief Sets size constraints for the window.
		 *
		 * Limits the window's resizable dimensions. Use `-1` for any parameter to indicate no limit.
		 */
		void SetWindowSizeLimits(int MinWidth, int MinHeight, int MaxWidth, int MaxHeight);

		/**
		 * @brief Sets the cursor's position within the window.
		 *
		 * Moves the cursor to the specified position relative to the upper-left corner of the window.
		 * @param XPos The X coordinate of the new cursor position.
		 * @param YPos The Y coordinate of the new cursor position.
		 */
		void SetCursorPosition(double XPos, double YPos);


		/**
		 * @brief Hides the window completely from the user's view.
		 *
		 * This function only applies to non-fullscreen windows (e.g., windowed or borderless windowed modes).
		 */
		void HideWindow();

		/**
		 * @brief Makes the window visible to the user.
		 *
		 * This function only applies to non-fullscreen windows (e.g., windowed or borderless windowed modes).
		 */
		void ShowWindow();

		/**
		 * @brief Forces the window to gain focus.
		 *
		 * Use this method sparingly, as it may disrupt the user's workflow. It should
		 * ideally be accompanied by an attention request if supported by the platform.
		 */
		void ForceFocusWindow();

		/**
		 * @brief Notifies the user of an event without interrupting their workflow.
		 *
		 * For example, on Windows, this will change the taskbar icon's color.
		 */
		void RequestWindowAttention();

		/**
		 * @brief Minimizes the window while keeping it visible in the taskbar.
		 */
		void IconifyWindow();

		/**
		 * @brief Maximizes the window, including any system window borders.
		 *
		 * This function transitions the window to a maximized state where it fills the screen
		 * but retains the system window borders (title bar, resize handles, etc.). Unlike fullscreen mode,
		 * the window remains a standard desktop window and does not occupy the entire display exclusively
		 * (e.g., taskbars and other desktop elements are still visible).
		 */
		void MaximizeWindow();

		/**
		 * @brief Resets the window to its initial state.
		 *
		 * The "state" refers to the window's scale, size, position, and any other visual attributes
		 * configured during the `Init` call. This function ensures the window returns to its original
		 * appearance and placement as defined at creation.
		 */
		void ResetWindowState();

		/**
		 * @brief Resets the window's icon to its default.
		 *
		 * Clears any custom window icon and reverts to the default application icon.
		 */
		void ResetWindowIcon();

		/**
		 * @brief Updates the window type and optionally its dimensions.
		 *
		 * This function allows transitioning the window to a new type (e.g., fullscreen or windowed)
		 * and optionally updates the width and height.
		 * @param NewWindowType The desired new window type.
		 * @param WindowWidth Optional. The new width for the window.
		 * @param WindowHeight Optional. The new height for the window.
		 */
		void UpdateWindowType(WindowType NewWindowType,
			std::optional<int> WindowWidth = std::optional<int>{ std::nullopt },
			std::optional<int> WindowHeight = std::optional<int>{ std::nullopt }
		);

		/**
		 * @brief Terminates the window and cleans up resources.
		 *
		 * Call this function before exiting the application to ensure proper cleanup of resources
		 * associated with the window.
		 */
		void Terminate();

	protected:
		/**
		 * @brief Registers the current instance with the SystemManager to ensure only one instance
		 * of the class exists and is globally accessible after successful initialization.
		 */
		virtual void RegisterWithSystemManager() override final;
		
		/**
		 * @brief Called when the window has been successfully initialized.
		 *
		 * This function is triggered once the window has been created and initialized.
		 * You can override this function to perform additional setup or initialization
		 * for your application.
		 */
		virtual void OnInitialized();

		/**
		 * @brief Cleans up resources and terminates the window.
		 *
		 * This function is called when the window is about to be terminated. It is
		 * intended for releasing any allocated resources and performing necessary
		 * cleanup tasks. This function will not return.
		 */
		virtual void Termination();

		/**
		 * @brief Applies specific window hints before window creation.
		 *
		 * This function allows you to apply custom window hints based on the window
		 * type (e.g., transparency, resolution, etc.). Refer to the GLFW window hints
		 * documentation for all possible hints. Override this function to configure
		 * the window with specific properties before creation.
		 * @param WindowType The type of the window to configure hints for.
		 */
		virtual void ApplyWindowHints(WindowType WindowType);

		/**
		 * @brief Registers custom callbacks for window events.
		 *
		 * Override this function to register your custom callback functions for window
		 * events. This function provides the flexibility to add custom behavior
		 * based on your application's needs.
		 */
		virtual void OnRegisterCallbacks();

		/**
		 * @brief Handles framebuffer size changes.
		 *
		 * This function is called when the framebuffer size changes (e.g., on window
		 * resize). The default implementation sets the OpenGL view to match the new
		 * size, but you can override it to apply custom behavior.
		 * @param Width The new width of the framebuffer.
		 * @param Height The new height of the framebuffer.
		 */
		virtual void OnFrameBufferSizeChange(GLFWwindow* Window, int Width, int Height);

		/**
		 * @brief Handles window focus changes.
		 *
		 * Override this to manage window focus changes
		 * (e.g., pause/resume game or application logic).
		 */
		virtual void OnWindowFocusChange(GLFWwindow* Window, bool Focused);

		/**
		 * @brief Handles window close events.
		 *
		 * This function is triggered when the user attempts to close the window (e.g., by clicking the close button).
		 * By default, the window will close, but you can override this function to prevent closure or prompt the user
		 * for confirmation. Use glfwSetWindowShouldClose(Window, GLFW_FALSE) within this function to cancel the close event.
		 *
		 * @param Window The window that received the close event.
		 */
		virtual void OnWindowClose(GLFWwindow* Window);

		/**
		 * @brief Handles window size changes.
		 *
		 * You can use this function to handle layout updates
		 * or resource adjustments when the window is resized.
		 * @param Width The new width of the window.
		 * @param Height The new height of the window.
		 */
		virtual void OnWindowSizeChange(GLFWwindow* Window, int Width, int Height);

		/**
		 * @brief Handles window position changes.
		 *
		 * This function is triggered when the window's position changes (e.g., when
		 * moved to another screen or changed via user interaction). You can override
		 * this to handle positioning-related tasks.
		 * @param XPos The new X position of the window relative to the Left.
		 * @param YPos The new Y position of the window relative to the Top.
		 */
		virtual void OnWindowPositionChange(GLFWwindow* Window, int XPos, int YPos);

		/**
		 * @brief Handles changes in window content scale.
		 *
		 * This function is called when the window's content scale changes, which may
		 * occur when the window is moved to a monitor with a different DPI scaling.
		 * You can override this to adjust your rendering or UI elements accordingly.
		 * @param XScale The new horizontal scaling factor.
		 * @param YScale The new vertical scaling factor.
		 */
		virtual void OnWindowContentScaleChange(GLFWwindow* Window, float XScale, float YScale);

		/**
		 * @brief Handles window iconification events.
		 *
		 * This function is called when the window is iconified (minimized) or restored.
		 * You can override this to pause or resume application logic based on the window's
		 * iconified state.
		 * @param Iconified A boolean indicating whether the window is iconified or restored.
		 */
		virtual void OnWindowIconify(GLFWwindow* Window, bool Iconified);

		/**
		 * @brief Handles window maximization events.
		 *
		 * This function is called when the window is maximized or restored. Override
		 * this function to handle custom logic when the window is maximized or restored.
		 * @param Maximized A boolean indicating whether the window is maximized or restored.
		 */
		virtual void OnWindowMaximize(GLFWwindow* Window, bool Maximized);

		/**
		 * @brief Handles window refresh events.
		 *
		 * This function is called when the window needs to be refreshed. On certain
		 * systems (e.g., Aero, Compiz, Aqua), this may only occur when the window
		 * or framebuffer is resized. Override this to handle any necessary refresh logic.
		 */
		virtual void OnWindowRefreshChange(GLFWwindow* Window);

		/**
		 * @brief Handles cursor entering or leaving the window.
		 *
		 * Override this function to manage interactions or input focus when the cursor is inside
		 * or outside the window.
		 * @param Entered A boolean indicating whether the cursor entered (true) or left
		 *                (false) the window.
		 */
		virtual void OnCursorEnterWindow(GLFWwindow* Window, int Entered);

	private:

		/**
		 * @brief Sets up the OpenGL context for the window.
		 *
		 * Ensures the window is configured to use the correct OpenGL version and context.
		 */
		void SetOpenGLContext();

		/**
		* @brief Configures input modes for the window.
		*
		* Sets input-related behaviors such as cursor visibility and locking.
		*/
		void SetupInputModes();

		/**
		* @brief Registers all GLFW event callbacks for the window.
		*
		* Ensures proper handling of events such as resizing, focus, and mouse input.
		*/
		void RegisterCallbacks();

		/**
		 * @brief Handles focus changes for the window.
		 *
		 * Adjusts window state and behavior based on focus changes.
		 * @param Focus Whether the window is focused or not.
		 */
		void HandleFocus(bool Focus);

		/**
		* @brief Creates and initializes a new GLFW window.
		*
		* Sets up the GLFW window with the specified configuration.
		*/
		GLFWwindow* CreateWindow(
			std::string_view ApplicationName,
			WindowType WindowType,
			std::optional<int> WindowWidth,
			std::optional<int> WindowHeight
		);


		/**
		 * @brief Handles focus changes for the window.
		 *
		 * Triggered when the window gains or loses focus.
		 * @param Window The GLFW window instance.
		 * @param Focused Indicates if the window is focused (true) or not (false).
		 */
		void WindowFocusCallback(GLFWwindow* Window, bool Focused);

		/**
		 * @brief Handles mouse button events for the window to manage window focus.
		 *
		 * Triggered when a mouse button is pressed or released.
		 * @param Window The GLFW window instance.
		 * @param Button The mouse button that triggered the event.
		 * @param Action The action (press or release).
		 * @param Mods Modifier keys active during the event.
		 */
		void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods);

		/**
		 * @brief Sets GLFW window hints for configuration.
		 *
		 * Configures properties such as OpenGL version and window type.
		 * @param OpenGLMajorVersion Major version of OpenGL to use.
		 * @param OpenGLMinorVersion Minor version of OpenGL to use.
		 * @param WindowType Type of window (e.g., fullscreen, windowed).
		 */
		void ApplyWindowHints(int OpenGLMajorVersion, int OpenGLMinorVersion, WindowType WindowType);

		/**
		 * @brief Static wrapper for GLFW event callbacks.
		 *
		 * These are static wrapper functions that call member functions on the `BaseWindow` class.
		 * They handle various events such as window resizing, focus changes, mouse input, and cursor events.
		 * They are called by GLFW whenever these events occur on the associated window.
		 */
		static void glfwFrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height);
		static void glfwWindowFocusCallback(GLFWwindow* Window, int Focused);
		static void glfwWindowCloseCallback(GLFWwindow* Window);
		static void glfwWindowSizeCallback(GLFWwindow* Window, int Width, int Height);
		static void glfwWindowPosCallback(GLFWwindow* Window, int XPos, int YPos);
		static void glfwWindowContentScaleCallback(GLFWwindow* Window, float XScale, float YScale);
		static void glfwWindowIconifyCallback(GLFWwindow* Window, int Iconified);
		static void glfwWindowMaximizeCallback(GLFWwindow* Window, int Maximized);
		static void glfwWindowRefreshCallback(GLFWwindow* Window);
		static void glfwCursorEnterCallback(GLFWwindow* Window, int Entered);
		static void glfwMouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods);
		static void glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
		static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	
	private:
		bool m_WindowFocused = true;			///< Tracks the focus state of the window.
		GLFWwindow* m_CurrentWindow = nullptr;	///< Pointer to the GLFW window instance.
	};

} // namespace fgl