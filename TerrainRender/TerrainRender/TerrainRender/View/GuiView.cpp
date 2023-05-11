#include "GuiView.h"
#include <commdlg.h>
#include "../resource.h"
#include "../ImGui/imguistyleserializer.h" 
#include "../StringConverter.h"
#include "../ImGui/imgui_internal.h"
#include <iomanip>
#include <ctime>
#include "../ErrorHandler.h"

const static float PI = 3.14159265358979323846f;

bool ToggleButton(const char* str_id, bool* isOn)
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    bool isClicked = false;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked()) 
    {
        *isOn = !*isOn;
        isClicked = true;
    }
    ImGuiContext& gg = *GImGui;
    float ANIM_SPEED = 0.085f;
    if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))
        float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
    if (ImGui::IsItemHovered())
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*isOn ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
    else
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*isOn ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + (*isOn ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));

    ImGui::SameLine();

    if(*isOn)
        ImGui::Text("Flythrough");
    else
        ImGui::Text("Explore 3D");

    return isClicked;

}


bool GuiView::Initalize(Microsoft::WRL::ComPtr<ID3D11Device> m_device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext, IControllerPtr controller)
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
    result = ImGui_ImplDX11_Init(m_device.Get(), m_deviceContext.Get());
    return result;
}

void GuiView::DisplayWindows()
{
    m_isFlythroughModeOn = m_terrainController->HandleMessage(IDCC_IS_FLYTHROUGH_MODE_ON, {}, {});
    if (m_show_GeneralWin)
        GeneralWindow();
    if (m_show_HelpWindow)
        Help();
    if (m_show_FlythroughWin && m_isFlythroughModeOn)
        FlythroughWindow();
    if (m_show_Explore3DWin && !m_isFlythroughModeOn)
        Explore3DWindow();
}

void GuiView::Help()
{
    ImGui::Begin("Help", &m_show_HelpWindow, 0);

    try {
        ImGui::SeparatorText("Steps");

        std::string text = R"(To use this program, follow these steps:

1. Load the meshes by selecting either "File > Open Terrain" or "File > Open Project" from the menu. You can choose to load the meshes with either soft or sharp edges. Soft edges are for interpolated shading, while sharp edges are for flat shading. The default mode is "Explore3D", where you can move the camera using the keyboard.

2. Load a trajectory file by selecting "File > Open Trajectory". After the file is successfully loaded, you can switch to "Flythrough" mode, where you can play and record the camera path.

3. Load a configuration file by selecting "File > Open configuration file" or set the position of the meshes and trajectory using the graphical user interface.

4. Set the output directory path by selecting "File > Set output directory" where you want to save the frames of the picture.)";
        ImGui::TextWrapped(text.c_str());

        ImGui::SeparatorText("Controls in Explore 3D mode");
        if (ImGui::BeginTable("Controls Explore 3D", 2))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("W"); ImGui::TableSetColumnIndex(1); ImGui::Text("Move forward");

                ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("S"); ImGui::TableSetColumnIndex(1); ImGui::Text("Move backward");

                ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("A"); ImGui::TableSetColumnIndex(1); ImGui::Text("Move left");

                ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("D"); ImGui::TableSetColumnIndex(1); ImGui::Text("Move right");

                ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("C"); ImGui::TableSetColumnIndex(1); ImGui::Text("Move down");

                ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("SPACE"); ImGui::TableSetColumnIndex(1); ImGui::Text("Move up");

                ImGui::EndTable();
        }

            ImGui::Separator();
        
        ImGui::TextWrapped("Camera Rotation: To rotate the camera, press and hold the left mouse button and move the mouse.");

        ImGui::SeparatorText("Controls in Flythrough mode");
        if (ImGui::BeginTable("Controls Flythrough", 2))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("R"); ImGui::TableSetColumnIndex(1); ImGui::Text("Start / Stop record");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("SPACE"); ImGui::TableSetColumnIndex(1); ImGui::Text("Play / Pause");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("ESC"); ImGui::TableSetColumnIndex(1); ImGui::Text("Stop");

            ImGui::EndTable();
        }

        ImGui::TextWrapped("The process of screen capturing with saving on the hard disk is resource-intensive. The recommended speed for screen capturing is below 0.5.");
    }
    catch (const TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (const ImGuiErrorLogCallback& e)
    {
        ImGui::ErrorCheckEndFrameRecover(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Caught unknown exception");
    }
    ImGui::End();
}

void GuiView::ShowHelp()
{
    m_show_HelpWindow = true;
}
void GuiView::ShowGeneralWindow()
{
    m_show_GeneralWin = true;
}
void GuiView::ShowExplore3DWindow()
{
    m_show_Explore3DWin = true;
}
void GuiView::ShowFlythroughWindow()
{
    m_show_FlythroughWin = true;
}


void GuiView::GeneralWindow()
{
    try
    {

        ImGui::PushItemWidth(ImGui::GetFontSize() * -15);
        ImGui::Begin("General Settings", &m_show_GeneralWin, 0);

        if (ToggleButton("Mode", &m_isFlythroughModeOn))
        {
            bool success = true;
            if (m_isFlythroughModeOn)
                success = this->m_terrainController->HandleMessage(IDC_ACTIVATE_FLYTHROUGH_MODE, {}, {});
            else
                success = this->m_terrainController->HandleMessage(IDC_ACTIVATE_3DEXPLORE_MODE, {}, {});
            if (!success)
            {
                m_isFlythroughModeOn = false;
                THROW_TREXCEPTION(L"To change to Flythrough mode, please load a trajectory file by going to 'File > Load trajectory' as the current trajectory file has not been loaded yet.");
            }
        }

        if (ImGui::CollapsingHeader("Origo LLA coordinates"))
        {
            if (ImGui::InputFloat("Longitude", &m_explore3dState.origo.longitude, 0.0f, 0.0f, "%.4f"))
            {
                m_terrainController->HandleMessage(IDC_ORIGO_SET_LONGITUDE, { m_explore3dState.origo.longitude }, {  });
            }
            if (ImGui::InputFloat("Latitude", &m_explore3dState.origo.latitude, 0.0f, 0.0f, "%.4f"))
            {
                m_terrainController->HandleMessage(IDC_ORIGO_SET_LATITUDE, { m_explore3dState.origo.latitude }, {  });
            }
        }
        if (ImGui::CollapsingHeader("View"))
        {
            if (ImGui::Checkbox("Shading", &m_generalState.isShadingOn ))
            {
                m_terrainController->HandleMessage(IDC_PIXELSHADER_SET_SHADING, {  }, { m_generalState.isShadingOn });
            }
            if (ImGui::Checkbox("Show grid", &m_generalState.isGridSeen))
            {
                m_terrainController->HandleMessage(IDC_XZ_PLANE_GRID_SET_ISSEEN, {  }, { m_generalState.isGridSeen });
            }
        }


        if (ImGui::CollapsingHeader("Camera properties"))
        {
            float fov = m_generalState.fieldOfView;
            if (ImGui::SliderFloat("Field of view", &fov, 0.5, /*0.01*/ PI, "%.3f"))
            {
                this->m_terrainController->HandleMessage(IDC_SET_CAMERA_FIELD_OF_VIEW, { fov }, {});
            }
            float nearScreen = m_generalState.screenNear;
            if (ImGui::SliderFloat("Near screen", &nearScreen, 0.5, /*0.01*/ 5, "%.3f"))
            {
                this->m_terrainController->HandleMessage(IDC_SET_CAMERA_NEAR_SCREEN, { nearScreen }, {});
            }
            float farScreen = m_generalState.screenDepth;
            if (ImGui::SliderFloat("Far screen", &farScreen, 10, /*0.01*/ 3000, "%.3f"))
            {
                this->m_terrainController->HandleMessage(IDC_SET_CAMERA_FAR_SCREEN, { farScreen }, {});
            }

            if (ImGui::Button("Reset camera position"))
            {
                this->m_terrainController->HandleMessage(IDC_E3D_CAMERA_RESET, {}, {});
            }
        }
        if (ImGui::CollapsingHeader("Meshes"))
        {
            ImGui::SeparatorText("Scale");
            if (ImGui::DragFloat("scale", &m_meshGroupState.scale.x, (0.01f), 0.0f, 100.0f))
            {
                m_terrainController->HandleMessage(IDC_MESH_GROUP_SCALE, { m_meshGroupState.scale.x }, {  });
            }

            ImGui::SeparatorText("Rotation [radian]");
            Vector3D& rotation = m_meshGroupState.rotation;
            if (ImGui::DragFloat3("[pitch, yaw, roll]", &rotation.x, (0.01f), -PI, PI))
                m_terrainController->HandleMessage(IDC_MESH_GROUP_ROTATION, { rotation.x, rotation.y, rotation.z }, {});


            ImGui::SeparatorText("Translation");
            Vector3D& translation = m_meshGroupState.translation;
            if (ImGui::DragFloat3("[x, y, z]", &translation.x, 0.1f))
            {
                m_terrainController->HandleMessage(IDC_MESH_GROUP_TRANSLATION, { translation.x, translation.y, translation.z }, {});
            }


            ImGui::SeparatorText("List of meshes");

            if (ImGui::Button("Clear meshes"))
            {
                m_terrainController->HandleMessage(IDC_BUTTON_CLEAR_MESHES, {}, {});
            }

            TerrainListBox();
        }
        if (ImGui::CollapsingHeader("Trajectory"))
        {
            if (!m_flythroughState.trajectoryPolyLine.empty())
            {

                TrajectoryState& trajectoryState = m_flythroughState.trajectoryPolyLine.at(0);
                std::string prefix = trajectoryState.isSeen ? "" : "* ";
                std::string polylineName = prefix + StringConverter::WideToString(trajectoryState.name);

                ImGui::SeparatorText("Loaded trajectory");
                if (ImGui::Button("Clear trajectory"))
                {
                    m_terrainController->HandleMessage(IDC_BUTTON_CLEAR_TRAJECTORY, {}, {});
                }
                if (ImGui::Button(polylineName.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                    ImGui::OpenPopup("Trajectory");
                }
                TrajectoryPopUp();
            }

        }
    }
    catch (const TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (const ImGuiErrorLogCallback& e)
    {
        ImGui::ErrorCheckEndFrameRecover(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Caught unknown exception");
    }

    ImGui::End();
}



std::vector<std::string> GuiView::CollectTerrainIDNames(void)
{
    std::vector<std::string> names;
   
    for (const MeshState& info : m_meshGroupState.Meshes)
    {
        std::string prefix = info.isSeen ? "" : "* ";
        std::string id = prefix + StringConverter::WideToString(info.name) + ", id: " + std::to_string(info.id);
        names.push_back(id);
    }
    return names;

}

void GuiView::TerrainPopUp(MeshState& state)
{
    if (ImGui::BeginPopup("Meshes")) //BeginPopupContextItem())
    {
        if (ImGui::Checkbox("Show mesh", &state.isSeen))
        {
            float b = (float)state.isSeen;
            m_terrainController->HandleMessage(IDC_MESH_SET_ISSEEN, { b }, { state.id });
        }

         ImGui::SeparatorText("Color");
         if (ImGui::ColorEdit4("color", &state.color.x))
         {
             m_terrainController->HandleMessage(IDC_MESH_SET_COLOR, { state.color.x, state.color.y, state.color.z, state.color.w }, { state.id });
         }
        ImGui::EndPopup();
    }
}

void GuiView::TrajectoryPopUp()
{
    if (ImGui::BeginPopup("Trajectory")) //BeginPopupContextItem())
    {
        if (!m_flythroughState.trajectoryPolyLine.empty())
        {
            bool &isSeen = m_flythroughState.trajectoryPolyLine.at(0).isSeen;
            if (ImGui::Checkbox("Show trajectory", &isSeen))
            {
                float b = (float)isSeen;
                m_terrainController->HandleMessage(IDC_TRAJECTORY_SET_ISSEEN, { b }, {  });
            }

            ImGui::SeparatorText("Rotation [radian]");
            Vector3D& rotation = m_flythroughState.trajectoryPolyLine.at(0).rotation;
            if (ImGui::DragFloat3("[pitch, yaw, roll]", &rotation.x, (0.01f), -PI, PI))
            {
                m_terrainController->HandleMessage(IDC_TRAJECTORY_ROTATION, { rotation.x, rotation.y ,rotation.z }, {});
            }


            ImGui::SeparatorText("Translation");
            Vector3D& translation = m_flythroughState.trajectoryPolyLine.at(0).translation;
            if (ImGui::DragFloat3("[x, y, z]", &translation.x, 0.1f))
            {
                m_terrainController->HandleMessage(IDC_TRAJECTORY_TRANSLATION, { translation.x, translation.y ,translation.z }, {});
            }

        }

        ImGui::EndPopup();
    }
}

void GuiView::TerrainListBox()
{
    static int item_current_idx = 0; // Here we store our selection data as an index.
    if (ImGui::BeginListBox("Meshes"))
    {
        std::vector<std::string> terrainIds = CollectTerrainIDNames();
        if (item_current_idx >= terrainIds.size())
        {
           item_current_idx = 0;
        }
        for (int n = 0; n < terrainIds.size(); n++)
        {
            ImGui::PushID(n);
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(terrainIds.at(n).c_str(), is_selected))
            {
                item_current_idx = n;
                ImGui::OpenPopup("Meshes");
            }
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }

            this->TerrainPopUp(m_meshGroupState.Meshes.at(item_current_idx));
            ImGui::PopID();
        }
        ImGui::EndListBox();
    }
}


template <class T>
void PrintStatus(const T& state)
{
    ImGui::Spacing();
    ImGui::SeparatorText("Position");
    if (ImGui::BeginTable("Position", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("x");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("y");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("z");
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%.4f", state.currentCameraPosition.x);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.4f", state.currentCameraPosition.y);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.4f", state.currentCameraPosition.z);
        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::SeparatorText("Rotation [radian]");
    if (ImGui::BeginTable("Rotation", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("pitch");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("yaw");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("roll");
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%.4f", state.currentCameraRotation.x);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.4f", state.currentCameraRotation.y);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.4f", state.currentCameraRotation.z);
        ImGui::EndTable();
    }

    ImGui::SeparatorText("Origo LLA coordinates");
    {
        if (ImGui::BeginTable("Origo LLA", 2))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("latitude");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("longitude");
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%.4f", state.origo.latitude);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.4f", state.origo.longitude);
            ImGui::EndTable();
        }
    }
    ImGui::SeparatorText("Date and time");
    {
        std::time_t time(state.currentEpochTime.getSeconds());
        std::tm tm;
        gmtime_s(&tm, &time);
        char buffer[80];
        std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &tm);
        ImGui::Text("UTC: %s", buffer);
        ImGui::Text("Unix time: %d", state.currentEpochTime.getSeconds());

    }
    ImGui::SeparatorText("Sun position");
    {
        if (ImGui::BeginTable("Sun position", 2))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("azimuth");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("elevation");
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%.4f", state.currentSunPosition.azimuth);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.4f", state.currentSunPosition.elevation);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%.4f", state.currentSunPosition.azimuth / PI * 180.0);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.4f", state.currentSunPosition.elevation / PI * 180.0);
            ImGui::EndTable();
        }
    }
}

void GuiView::FlythroughWindow()
{
    try
    {
        ImGui::PushItemWidth(ImGui::GetFontSize() * -15);
        ImGui::Begin("Flythrough Settings", &m_show_FlythroughWin, 0);
        ImGui::SeparatorText("Simulation control");
        if (ImGui::Button("Play"))
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_START, {}, {});

        ImGui::SameLine();
        if (ImGui::Button("Pause"))
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_PAUSE, {}, {});

        ImGui::SameLine();
        if (ImGui::Button("Stop"))
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_STOP, {}, {});      

        ImGui::SeparatorText("Record control");
        std::wstring str = L"Output directory: " + m_outputDir;
        ImGui::TextWrapped(StringConverter::WideToString(str).c_str());
        
        if (ImGui::Button(m_isRecordingOn ? "Stop Record" : "Record Frames"))
        {
            if(!m_isRecordingOn)
                this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_RECORD_START, {}, {});
            else
                this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_RECORD_STOP, {}, {});
        }

        ImGui::SeparatorText("Properties");
        static float flythrough_speed = m_flythroughState.speed;
        if (ImGui::SliderFloat("Speed", &flythrough_speed, 0.1f, 3.0f, "%.3f"))
        {
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_SET_SPEED, { flythrough_speed }, {});
        }
        
        m_frame = m_flythroughState.currentFrame + 1;
        ImGui::Text("Frame: %d / %d", m_frame, m_flythroughState.numberOfFrame);
        if (ImGui::SliderInt("Frames", &m_frame, 1, max(1,m_flythroughState.numberOfFrame)))
        {
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_SET_FRAME, {}, { (unsigned)(m_frame - 1) });
        }
        std::string unixtimestr = std::to_string(m_flythroughState.startEpochTime.getSeconds());
        char ut[11];
        strcpy_s<11>(ut, unixtimestr.c_str());
        ImGui::PushID("UnixTime_FT");
        if (ImGui::InputText("Unix time", ut, 11))
        {
            unsigned newUnix = std::atol(ut);
            if (newUnix > 0)
            {
                this->m_terrainController->HandleMessage(IDC_SET_START_TIME_TRAJECTORY, {}, {newUnix});
            }
        }      
        ImGui::PopID();
        PrintStatus<FlythroughState>(m_flythroughState);
    }
    catch (const TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (const ImGuiErrorLogCallback& e)
    {
        ImGui::ErrorCheckEndFrameRecover(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Caught unknown exception");
    }
    ImGui::End();
    
}
void GuiView::Explore3DWindow()
{
    try {
        ImGui::PushItemWidth(ImGui::GetFontSize() * -15);
        ImGui::Begin("Explore 3D Settings", &m_show_Explore3DWin, 0);

        ImGui::SeparatorText("FPS camera properties");
        float cameraSpeed = m_explore3dState.speed;
        if (ImGui::SliderFloat("speed", &cameraSpeed, 0.0f, /*0.01*/ 0.8f, "%.3f"))
        {
            this->m_terrainController->HandleMessage(IDC_E3D_CAMERA_SPEED, { cameraSpeed }, {});
        }
        float cameraRotationSpeed = m_explore3dState.rotationSpeed;
        if (ImGui::SliderFloat("rotation speed", &cameraRotationSpeed, 0.0001f, 0.002f, "%.4f"))
        {
            this->m_terrainController->HandleMessage(IDC_E3D_ROTATION_SPEED, { cameraRotationSpeed }, {});
        }

        std::string unixtimestr = std::to_string(m_explore3dState.currentEpochTime.getSeconds());
        char ut[11];
        strcpy_s<11>(ut, unixtimestr.c_str());
        ImGui::PushID("UnixTime_E3D");
        if (ImGui::InputText("Unix time", ut, 11))
        {
            unsigned newUnix = std::atol(ut);
            if (newUnix > 0)
            {
                this->m_terrainController->HandleMessage(IDC_SET_TIME_E3D, {}, { newUnix });
            }
        }
        ImGui::PopID();
        PrintStatus<Explore3DState>(m_explore3dState);
    }
    catch (const TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (const std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (const ImGuiErrorLogCallback& e)
    {
        ImGui::ErrorCheckEndFrameRecover(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Caught unknown exception");
    }
    ImGui::End();
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


void GuiView::HandleIModelState(const MeshGroupState& states)
{
    m_meshGroupState = states;
}

void GuiView::HandleIModelState(const FlythroughState& state)
{
    m_flythroughState = state;
}

void GuiView::HandleIModelState(const Explore3DState& state)
{
    m_explore3dState = state;
}
void GuiView::HandleIModelState(const GeneralModelState& state)
{
    m_generalState = state;
}

void GuiView::SetOutputDirectory(const std::wstring& dir)
{
    m_outputDir = dir;
}

void GuiView::SetIsRecordingOn(bool l)
{
    m_isRecordingOn = l;
}
