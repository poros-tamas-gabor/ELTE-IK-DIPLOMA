#include "GuiView.h"
#include <commdlg.h>
#include "../resource.h"
#include "../ImGui/imguistyleserializer.h" 

const static float PI = 3.14159265358979323846;
bool GuiView::Initalize(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, IController* controller)
{
    bool result;

    if (controller == nullptr)
    {
        return false;
    }
    this->m_terrainController = controller;
    // Setup Dear ImGui style
    ImGui::ResetStyle(28);

    // Setup Platform/Renderer backends
    result = ImGui_ImplDX11_Init(_device, _deviceContext);
    return result;
}

void GuiView::ShowSettingWindow()
{
    static bool show_setting_window = true;


    if (show_setting_window)
    {
        ImGui::Begin("Setting Window", &show_setting_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        static int style = 28;
        if (ImGui::SliderInt("Style", &style, 0, 38))
        {
            ImGui::ResetStyle(style);
        }

        if (ImGui::CollapsingHeader("File"))
        {
            ImGui::Separator();
            ImGui::Text("text");
            ImGui::Separator();
            
            if (ImGui::Button("Set Terrain Path"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FIlE_TERRAIN, NULL, NULL);
            }

            if (ImGui::Button("Set Trajectory Path"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FIlE_CAMERA_TRAJECTORY, NULL, NULL);
            }

        }

        if (ImGui::CollapsingHeader("Mode"))
        {
            static bool selected[2] = { true, false };
            const char* mode[2] = { "3DExplore", "Flythrough"};
            const unsigned msgs[2] = { IDC_BUTTON_3DEXPLORE_MODE, IDC_BUTTON_FLYTHROUGH_MODE };
            
            for (int i = 0; i < 2; i++)
            {
                ImGui::PushID(i);
                if (ImGui::Selectable(mode[i], selected + i, 0, ImVec2(50, 50)))
                {
                    this->m_terrainController->HandleMessage(msgs[i], NULL, NULL);
                    selected[(i + 1) % 2] ^= 1;
                }

                if (i == 0)
                    ImGui::SameLine();
                ImGui::PopID();
            }

        }

        if (ImGui::CollapsingHeader("Flythrough"))
        {
            if (ImGui::Button("Play"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FLYTHROUGH_START, NULL, NULL);
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FLYTHROUGH_PAUSE, NULL, NULL);
            }
            ImGui::SameLine();
            if (ImGui::Button("Stop"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FLYTHROUGH_STOP, NULL, NULL);
            }

            if (ImGui::Button("Record"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FLYTHROUGH_RECORD, NULL, NULL);
            }
            static float flythrough_speed = 1;
            if (ImGui::SliderFloat("Camera speed", &flythrough_speed, 0.5, 10, "%.3f"))
            {
                this->m_terrainController->HandleMessage(IDC_SLIDER_FLYTHROUGH_SPEED, &flythrough_speed, NULL);
            }

        }

        if (ImGui::CollapsingHeader("3D Explore"))
        {
            static float cameraSpeed = 0.005f;
            if (ImGui::SliderFloat("Camera speed", &cameraSpeed, 0, /*0.01*/ 1, "%.3f"))
            {
                this->m_terrainController->HandleMessage(IDC_SLIDER_CAMERA_SPEED, &cameraSpeed, NULL);
            }
            static float cameraRotationSpeed = 0.001f;
            if (ImGui::SliderFloat("Camera Rotation speed", &cameraRotationSpeed, 0, 0.002, "%.3f"))
            {
                this->m_terrainController->HandleMessage(IDC_SLIDER_CAMERA_ROTATION_SPEED, &cameraRotationSpeed, NULL);
            }
        }


        if (ImGui::CollapsingHeader("Camera Properties"))
        {

            if (ImGui::TreeNode("Projection Properties"))
            {
                static float fov = PI / 2;
                if (ImGui::SliderFloat("Field of view", &fov, 1, /*0.01*/ PI, "%.3f"))
                {
                    this->m_terrainController->HandleMessage(IDC_SLIDER_PROJECTION_FIELD_OF_VIEW, &fov, NULL);
                }
                static float nearScreen = 1;
                if (ImGui::SliderFloat("NearScreen", &nearScreen, 0.5, /*0.01*/ 5, "%.3f"))
                {
                    this->m_terrainController->HandleMessage(IDC_SLIDER_PROJECTION_NEAR_SCREEN, &nearScreen, NULL);
                }
                static float farScreen = 1000;
                if (ImGui::SliderFloat("FarScreen", &farScreen, 10, /*0.01*/ 3000, "%.3f"))
                {
                    this->m_terrainController->HandleMessage(IDC_SLIDER_PROJECTION_FAR_SCREEN, &farScreen, NULL);
                }
                ImGui::TreePop();
            }

            if (ImGui::Button("Reset Camera"))
            {

                this->m_terrainController->HandleMessage(IDC_BUTTON_CAMERA_RESET, NULL, NULL);
            }
        }




   
        ImGui::End();
    }

}
void GuiView::BeginFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}
void GuiView::EndFrame()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GuiView::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    
}