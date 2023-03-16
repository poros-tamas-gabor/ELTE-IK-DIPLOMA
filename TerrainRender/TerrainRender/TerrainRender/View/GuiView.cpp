#include "GuiView.h"
#include <commdlg.h>
#include "../Controller/ControllerEvents.h"


bool GuiView::Initalize(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, IController* controller)
{
    bool result;

    if (controller == nullptr)
    {
        return false;
    }
    this->m_terrainController = controller;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    result = ImGui_ImplDX11_Init(_device, _deviceContext);
    return result;
}

void GuiView::ShowSettingWindow()
{
    static bool show_another_window = true;

    static wchar_t terrainFilePath[260];       // buffer for file name
    static wchar_t trajectoryFilePath[260];       // buffer for file name

    OPENFILENAME ofn;       // common dialog box structure
    HWND hwnd = NULL;              // owner window
    HANDLE hf;              // file handle

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = terrainFilePath;
    //
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    //
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(terrainFilePath);
    ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 


    if (show_another_window)
    {
        ImGui::Begin("Setting Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        if (ImGui::Button("Set Terrain Path"))
        {
            if (GetOpenFileName(&ofn) == TRUE)
            {
                ControllerEvent::TerrainFileSelectEvent event;
                event.SetFilePath(terrainFilePath);
                this->m_terrainController->Control(&event);
            }
        }
       //size_t i;
       //char text[260];
       //
       //wcstombs_s(&i, text, terrainFilePath, 260);
       //ImGui::Text(text);

        ofn.lpstrFile = trajectoryFilePath;

        if (ImGui::Button("Set Trajectory Path"))
        {
            if (GetOpenFileName(&ofn) == TRUE)
            {

            }

        }
        static float speeds[2] = { 50, 50 };
        if (ImGui::SliderFloat2("Set Camera Speed and Rotation Speed", speeds, 0, 100))
        {
            ControllerEvent::EnterFloatEvent event;
            event.SetEventType(ControllerEvent::EnterFloat_cameraMoveSpeed);
            event.SetFloatData(speeds[0]/10000);
            this->m_terrainController->Control(&event);


            event.SetEventType(ControllerEvent::EnterFloat_cameraRotationSpeed);
            event.SetFloatData(speeds[1] / 50000);
            this->m_terrainController->Control(&event);
        }

        if (ImGui::Button("Reset Camera"))
        {
            ControllerEvent::Event event;
            event.SetEventType(ControllerEvent::ClickButton_ResetCamera);
            this->m_terrainController->Control(&event);

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