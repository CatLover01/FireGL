#include <FireGL/Core/Window.h>
#include <FireGL/Core/InputManager.h>
#include <FireGL/Core/BaseLog.h>
#include <FireGL/Core/SystemManager.h>

#include <External/stb/stb_image.h>

namespace fgl {

    void BaseWindow::Initialize(int OpenGLMajorVersion, int OpenGLMinorVersion, std::string_view ApplicationName, WindowType WindowType, bool VSyncEnabled, std::optional<int> WindowWidth, std::optional<int> WindowHeight)
    {
        LOG_ASSERT(glfwInit(), "Failed to initialize GLFW");

        ApplyWindowHints(OpenGLMajorVersion, OpenGLMinorVersion, WindowType);
        m_CurrentWindow = CreateWindow(ApplicationName, WindowType, WindowWidth, WindowHeight);

        SetOpenGLContext();
        SetupInputModes();
        RegisterCallbacks();

        SetVSync(VSyncEnabled);
        OnInitialized();
        RegisterWithSystemManager();
    }

    void BaseWindow::ApplyWindowHints(int OpenGLMajorVersion, int OpenGLMinorVersion, WindowType WindowType)
    {
        ApplyWindowHints(WindowType);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLMinorVersion);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
    }

    GLFWwindow* BaseWindow::CreateWindow(std::string_view ApplicationName, WindowType WindowType, std::optional<int> WindowWidth, std::optional<int> WindowHeight)
    {
        GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* Mode = glfwGetVideoMode(Monitor);
        const int ScreenWidth = Mode->width;
        const int ScreenHeight = Mode->height;
        GLFWwindow* Window = nullptr;

        switch (WindowType)
        {
        case WindowType::FullScreen:
        {
            Window = glfwCreateWindow(ScreenWidth, ScreenHeight, ApplicationName.data(), Monitor, nullptr);
            break;
        }
        case WindowType::WindowedFullScreen:
        {
            glfwWindowHint(GLFW_RED_BITS, Mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, Mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, Mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, Mode->refreshRate);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            Window = glfwCreateWindow(ScreenWidth, ScreenHeight, ApplicationName.data(), nullptr, nullptr);
            break;
        }
        case WindowType::Windowed:
        {
            LOG_ASSERT((WindowWidth.has_value() && WindowHeight.has_value()), "Missing at least one Window's Coordinate");
            Window = glfwCreateWindow(WindowWidth.value(), WindowHeight.value(), ApplicationName.data(), nullptr, nullptr);
            break;
        }
        case WindowType::BorderlessWindowed:
        {
            LOG_ASSERT((WindowWidth.has_value() && WindowHeight.has_value()), "Missing at least one Window's Coordinate");

            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            Window = glfwCreateWindow(WindowWidth.value(), WindowHeight.value(), ApplicationName.data(), nullptr, nullptr);
            break;
        }
        default:
            LOG_ASSERT(false, "No Enum Type matched...");
        }

        LOG_ASSERT_CALLBACK(Window, "Failed to create a GLFW window", []() {
            glfwTerminate();
            });

        return Window;
    }

    void BaseWindow::SetOpenGLContext()
    {
        glfwSetWindowUserPointer(m_CurrentWindow, this);
        glfwMakeContextCurrent(m_CurrentWindow);
        LOG_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");

    }

    void BaseWindow::SetupInputModes()
    {
        glfwSetInputMode(m_CurrentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(m_CurrentWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
    }

    void BaseWindow::RegisterCallbacks()
    {
        glfwSetFramebufferSizeCallback(m_CurrentWindow, glfwFrameBufferSizeCallback);
        glfwSetWindowFocusCallback(m_CurrentWindow, glfwWindowFocusCallback);
        glfwSetWindowCloseCallback(m_CurrentWindow, glfwWindowCloseCallback);
        glfwSetWindowSizeCallback(m_CurrentWindow, glfwWindowSizeCallback);
        glfwSetWindowPosCallback(m_CurrentWindow, glfwWindowPosCallback);
        glfwSetWindowContentScaleCallback(m_CurrentWindow, glfwWindowContentScaleCallback);
        glfwSetWindowIconifyCallback(m_CurrentWindow, glfwWindowIconifyCallback);
        glfwSetWindowMaximizeCallback(m_CurrentWindow, glfwWindowMaximizeCallback);
        glfwSetWindowRefreshCallback(m_CurrentWindow, glfwWindowRefreshCallback);
        glfwSetCursorEnterCallback(m_CurrentWindow, glfwCursorEnterCallback);
        glfwSetMouseButtonCallback(m_CurrentWindow, glfwMouseButtonCallback);
        glfwSetCursorPosCallback(m_CurrentWindow, glfwMouseMoveCallback);
        glfwSetScrollCallback(m_CurrentWindow, glfwScrollCallback);

        OnRegisterCallbacks();
    }

    void BaseWindow::HandleFocus(bool Focus)
    {
        if (Focus) {
            double XPos, YPos;
            int Width, Height;
            GetCursorPosition(XPos, YPos);
            GetWindowSize(Width, Height);

            if (XPos >= 0 && XPos <= Width && YPos >= 0 && YPos <= Height) {
                glfwSetInputMode(m_CurrentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                m_WindowFocused = true;
                return;
            }
        }

        glfwSetInputMode(m_CurrentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_WindowFocused = false;
    }

    void BaseWindow::SetVSync(bool VSyncEnabled)
    {
        glfwSwapInterval(static_cast<int>(VSyncEnabled));
    }

    void BaseWindow::SetWindowPositionCenter()
    {
        GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* Mode = glfwGetVideoMode(Monitor);

        int WindowWidth, WindowHeight;
        GetWindowSize(WindowWidth, WindowHeight);

        int CenterX = (Mode->width - WindowWidth) / 2;
        int CenterY = (Mode->height - WindowHeight) / 2;
        SetWindowPosition(CenterX, CenterY);
    }

    void BaseWindow::SetWindowPosition(int WidthFromLeft, int HeightFromTop)
    {
        glfwSetWindowPos(m_CurrentWindow, WidthFromLeft, HeightFromTop);
    }

    GLFWwindow* BaseWindow::GetWindowPtr() const
    {
        return m_CurrentWindow;
    }

    bool BaseWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_CurrentWindow);
    }

    bool BaseWindow::IsFocused() const
    {
        return m_WindowFocused;
    }

    void BaseWindow::Terminate()
    {
        Termination();
        glfwTerminate();
    }

    void BaseWindow::RegisterWithSystemManager()
    {
        SystemManager<BaseWindow>::Set(this);
    }

    const char* BaseWindow::GetWindowTitle() const
    {
        return glfwGetWindowTitle(m_CurrentWindow);
    }

    void BaseWindow::SetWindowTitle(std::string_view Title)
    {
        glfwSetWindowTitle(m_CurrentWindow, Title.data());
    }

    float BaseWindow::GetWindowOpacity() const
    {
        return glfwGetWindowOpacity(m_CurrentWindow);
    }

    float BaseWindow::GetAspectRatio() const
    {
        int Width, Height;
        GetWindowSize(Width, Height);

        return static_cast<float>(Width) / static_cast<float>(Height);
    }

    void BaseWindow::SetWindowOpacity(float Opacity)
    {
        glfwSetWindowOpacity(m_CurrentWindow, Opacity);
    }

    void BaseWindow::GetWindowSize(int& Width, int& Height) const
    {
        glfwGetFramebufferSize(m_CurrentWindow, &Width, &Height);
    }

    void BaseWindow::GetCursorPosition(double& XPos, double& YPos) const
    {
        glfwGetCursorPos(m_CurrentWindow, &XPos, &YPos);
    }

    void BaseWindow::SetWindowAspectRatio(int Width, int Height)
    {
        glfwSetWindowAspectRatio(m_CurrentWindow, Width, Height);
    }

    void BaseWindow::SetWindowIcon(std::string_view IconPath)
    {
        GLFWimage Image;
        Image.pixels = stbi_load(IconPath.data(), &Image.width, &Image.height, 0, 4); 
        if (Image.pixels)
        {
            glfwSetWindowIcon(m_CurrentWindow, 1, &Image);
            stbi_image_free(Image.pixels);
        }
        else
        {
            LOG_ERROR("Failed to load window icon from: " + std::string(IconPath), true)
        }
    }

    void BaseWindow::SetWindowSizeLimits(int MinWidth, int MinHeight, int MaxWidth, int MaxHeight)
    {
        glfwSetWindowSizeLimits(m_CurrentWindow, MinWidth == -1 ? GLFW_DONT_CARE : MinWidth,
            MinHeight == -1 ? GLFW_DONT_CARE : MinHeight, MaxWidth == -1 ? GLFW_DONT_CARE : MaxWidth,
            MaxHeight == -1 ? GLFW_DONT_CARE : MaxHeight);
    }

    void BaseWindow::SetCursorPosition(double XPos, double YPos)
    {
        glfwSetCursorPos(m_CurrentWindow, XPos, YPos);
    }

    void BaseWindow::HideWindow()
    {
        glfwHideWindow(m_CurrentWindow);
    }

    void BaseWindow::ShowWindow()
    {
        glfwShowWindow(m_CurrentWindow);
    }

    void BaseWindow::ForceFocusWindow()
    {
        glfwFocusWindow(m_CurrentWindow);
    }

    void BaseWindow::RequestWindowAttention()
    {
        glfwRequestWindowAttention(m_CurrentWindow);
    }

    void BaseWindow::IconifyWindow()
    {
        glfwIconifyWindow(m_CurrentWindow);
    }

    void BaseWindow::MaximizeWindow()
    {
        glfwMaximizeWindow(m_CurrentWindow);
    }

    void BaseWindow::ResetWindowState()
    {
        glfwRestoreWindow(m_CurrentWindow);
    }

    void BaseWindow::ResetWindowIcon()
    {
        glfwSetWindowIcon(m_CurrentWindow, 0, nullptr);
    }

    void BaseWindow::UpdateWindowType(WindowType NewWindowType, std::optional<int> WindowWidth, std::optional<int> WindowHeight)
    {
        GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* Mode = glfwGetVideoMode(Monitor);

        switch (NewWindowType)
        {
        case WindowType::FullScreen:
        {
            glfwSetWindowMonitor(m_CurrentWindow, Monitor, 0, 0, Mode->width, Mode->height, Mode->refreshRate);
            break;
        }
        case WindowType::WindowedFullScreen:
        {
            glfwSetWindowAttrib(m_CurrentWindow, GLFW_DECORATED, GLFW_FALSE); // Remove borders and title bar
            glfwSetWindowMonitor(m_CurrentWindow, nullptr, 0, 0, Mode->width, Mode->height, Mode->refreshRate);
            break;
        }
        case WindowType::Windowed:
        {
            LOG_ASSERT((WindowWidth.has_value() && WindowHeight.has_value()), "Missing at least one Window's Coordinate");
            glfwSetWindowMonitor(m_CurrentWindow, nullptr, 0, 0, WindowWidth.value(), WindowHeight.value(), Mode->refreshRate);
            break;
        }
        case WindowType::BorderlessWindowed:
        {
            LOG_ASSERT((WindowWidth.has_value() && WindowHeight.has_value()), "Missing at least one Window's Coordinate");

            glfwSetWindowAttrib(m_CurrentWindow, GLFW_DECORATED, false);
            glfwSetWindowMonitor(m_CurrentWindow, nullptr, 0, 0, WindowWidth.value(), WindowHeight.value(), Mode->refreshRate);
            break;
        }
        default:
            LOG_ASSERT(false, "No Enum Type matched...");
        }
    }

    void BaseWindow::OnInitialized()
    {
    }

    void BaseWindow::Termination()
    {
    }

    void BaseWindow::ApplyWindowHints(WindowType WindowType)
    {
    }

    void BaseWindow::OnRegisterCallbacks()
    {
    }

    void BaseWindow::OnFrameBufferSizeChange(GLFWwindow* Window, int Width, int Height)
    {
        glViewport(0, 0, Width, Height); ///< Default Implementation not called when overriden.
    }

    void BaseWindow::OnWindowFocusChange(GLFWwindow* Window, bool Focused)
    {
    }

    void BaseWindow::OnWindowClose(GLFWwindow* Window)
    {
    }

    void BaseWindow::OnWindowSizeChange(GLFWwindow* Window, int Width, int Height)
    {
    }

    void BaseWindow::OnWindowPositionChange(GLFWwindow* Window, int XPos, int YPos)
    {
    }

    void BaseWindow::OnWindowContentScaleChange(GLFWwindow* Window, float XScale, float YScale)
    {
    }

    void BaseWindow::OnWindowIconify(GLFWwindow* Window, bool Iconified)
    {
    }

    void BaseWindow::OnWindowMaximize(GLFWwindow* Window, bool Maximized)
    {
    }

    void BaseWindow::OnWindowRefreshChange(GLFWwindow* Window)
    {
    }

    void BaseWindow::OnCursorEnterWindow(GLFWwindow* Window, int Entered)
    {
    }

    void BaseWindow::WindowFocusCallback(GLFWwindow* Window, bool Focused)
    {
        HandleFocus(Focused);
        OnWindowFocusChange(Window, m_WindowFocused);
    }

    void BaseWindow::MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods)
    {
        if (Button == GLFW_MOUSE_BUTTON_LEFT && Action == GLFW_PRESS && !m_WindowFocused)
        {
            HandleFocus(true);
        }
    }

    void BaseWindow::glfwFrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->OnFrameBufferSizeChange(Window, Width, Height);
        }
    }

    void BaseWindow::glfwWindowFocusCallback(GLFWwindow* Window, int Focused)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->WindowFocusCallback(Window, static_cast<bool>(Focused));
        }
    }

    void BaseWindow::glfwWindowCloseCallback(GLFWwindow* Window)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->OnWindowClose(Window);
        }
    }

    void BaseWindow::glfwWindowSizeCallback(GLFWwindow* Window, int Width, int Height)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->OnWindowSizeChange(Window, Width, Height);
        }
    }

    void BaseWindow::glfwWindowPosCallback(GLFWwindow* Window, int XPos, int YPos)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->OnWindowPositionChange(Window, XPos, YPos);
        }
    }

    void BaseWindow::glfwWindowContentScaleCallback(GLFWwindow* Window, float XScale, float YScale)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->OnWindowContentScaleChange(Window, XScale, YScale);
        }
    }

    void BaseWindow::glfwWindowIconifyCallback(GLFWwindow* Window, int Iconified)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->OnWindowIconify(Window, static_cast<bool>(Iconified));
        }
    }

    void BaseWindow::glfwWindowMaximizeCallback(GLFWwindow* Window, int Maximized)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->OnWindowMaximize(Window, Maximized);
        }
    }

    void BaseWindow::glfwWindowRefreshCallback(GLFWwindow* Window)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->OnWindowRefreshChange(Window);
        }
    }

    void BaseWindow::glfwCursorEnterCallback(GLFWwindow* Window, int Entered)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->OnCursorEnterWindow(Window, Entered);
        }
    }

    void BaseWindow::glfwMouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(Window));
        if (Self) {
            Self->MouseButtonCallback(Window, Button, Action, Mods);
        }
    }

    void BaseWindow::glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(window)); 
        if (Self && Self->IsFocused()) {
            InputManager* Input = SystemManager<InputManager>::Get();
            if (Input)
            {
                Input->UpdateMouse(window, xpos, ypos);
            }
        }
    }

    void BaseWindow::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        BaseWindow* Self = static_cast<BaseWindow*>(glfwGetWindowUserPointer(window));
        if (Self && Self->IsFocused()) {
            InputManager* Input = SystemManager<InputManager>::Get();
            if (Input)
            {
                Input->UpdateScroll(xoffset, yoffset);
            }
        }
    }

} // namespace fgl