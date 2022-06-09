#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include <SimpleEngineCore/Input.hpp>
#include <SimpleEngineCore/Application.hpp>

class SimpleEngineEditor : public SimpleEngine::Application
{
    virtual void on_update() override
    {
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_W))
        {
            camera_position[2] -= 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_S))
        {
            camera_position[2] += 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_A))
        {
            camera_position[0] -= 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_D))
        {
            camera_position[0] += 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_E))
        {
            camera_position[1] += 0.01f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_Q))
        {
            camera_position[1] -= 0.01f;
        }

        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_UP))
        {
            camera_rotation[0] += 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_DOWN))
        {
            camera_rotation[0] -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_RIGHT))
        {
            camera_rotation[1] -= 0.5f;
        }
        if (SimpleEngine::Input::IsKeyPressed(SimpleEngine::KeyCode::KEY_LEFT))
        {
            camera_rotation[1] += 0.5f;
        }
    }

    virtual void on_ui_draw() override
    {
        ImGui::Begin("Editor");
        ImGui::SliderFloat3("camera position", camera_position, -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", camera_rotation, 0, 360.f);
        ImGui::Checkbox("Perspective camera", &perspective_camera);
        ImGui::End();
    }

    int frame = 0;
};


int main()
{
    auto pSimpleEngineEditor = std::make_unique<SimpleEngineEditor>();

    int returnCode = pSimpleEngineEditor->start(1024, 768, "SimpleEngine Editor");

    //std::cin.get();

    return returnCode;
}