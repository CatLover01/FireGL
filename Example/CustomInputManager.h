#pragma once

#include <FireGL/Core/InputManager.h>
#include <FireGL/Core/SystemManager.h>
#include <FireGL/Renderer/Scene.h>
#include <FireGL/Renderer/Shapes/Sphere.h>

#include <../Example/CustomCamera.h>

using namespace fgl;

class CustomInputManager : public InputManager
{
public:

    // Here you'd pass everything you need for input (i.e. Player, Camera...)
    CustomInputManager(std::shared_ptr<CustomCamera> Camera)
        :
        m_Camera{ Camera }
    {
    }

protected:
   
    virtual void OnInitialized() override
    {
        RegisterKeyEvent(GLFW_KEY_W, KeyEventType::OnPressed, []() {
            //std::cout << "W key pressed!" << std::endl;
            });
        RegisterKeyEvent(GLFW_KEY_W, KeyEventType::OnTriggered, []() {
            //std::cout << "W key triggered!" << std::endl;
            });
        RegisterKeyEvent(GLFW_KEY_W, KeyEventType::OnReleased, &CustomInputManager::test, this, 1);
    }

    void test(int x)
    {
        //std::cout << "Test has worked: " << x << std::endl;
    }

    virtual void OnMouseUpdate(double xpos, double ypos) override {
        m_Camera->UpdateRotationInput(xpos, ypos);
    }

    virtual void OnScrollUpdate(double xoffset, double yoffset) override {
        m_Camera->AddMovementSpeed(yoffset);
    }

    virtual void OnProcessInput() override
    {
        // Exit
        if (IsKeyTriggered(GLFW_KEY_ESCAPE))
        {
            SystemManager<BaseWindow>::Get()->Terminate();
        }
        
        // Forward/Backward
        if (IsKeyTriggered(GLFW_KEY_W))
        {
            m_Camera->ProcessMovementInput(CameraMovement::Forward, GetInputDeltaTime());
        }
        else if (IsKeyTriggered(GLFW_KEY_S))
        {
            m_Camera->ProcessMovementInput(CameraMovement::Backward, GetInputDeltaTime());
        }

        // Left/Right
        if (IsKeyTriggered(GLFW_KEY_A))
        {
            m_Camera->ProcessMovementInput(CameraMovement::Leftwards, GetInputDeltaTime());
        }
        else if (IsKeyTriggered( GLFW_KEY_D))
        {
            m_Camera->ProcessMovementInput(CameraMovement::Rightwards, GetInputDeltaTime());
        }
    }

private:
    std::shared_ptr<CustomCamera> m_Camera;  // Access to the camera or other game objects
};