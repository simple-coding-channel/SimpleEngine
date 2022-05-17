#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include <SimpleEngineCore/Application.hpp>

class SimpleEngineEditor : public SimpleEngine::Application
{
    virtual void on_update() override
    {
    //    std::cout << "Update frame: " << frame++ << std::endl;
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