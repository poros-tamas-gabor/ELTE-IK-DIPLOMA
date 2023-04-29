#include "GuiView.h"
#include <commdlg.h>
#include "../resource.h"
#include "../ImGui/imguistyleserializer.h" 
#include "../StringConverter.h"
#include "../ImGui/imgui_internal.h"
#include <iomanip>
#include <ctime>
#include "../ErrorHandler.h"
bool ToggleButton(const char* str_id, bool* isFlythroughOn, bool isActive)
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    bool isClicked = false;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked() && isActive) 
    {
        *isFlythroughOn = !*isFlythroughOn;
        isClicked = true;
    }
    ImGuiContext& gg = *GImGui;
    float ANIM_SPEED = 0.085f;
    if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))
        float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
    if (ImGui::IsItemHovered())
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*isFlythroughOn ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
    else
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*isFlythroughOn ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + (*isFlythroughOn ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));

    ImGui::SameLine();

    if(*isFlythroughOn)
        ImGui::Text("Flythrough");
    else
        ImGui::Text("Explore 3D");

    return isClicked;

}

const static float PI = 3.14159265358979323846f;
bool GuiView::Initalize(Microsoft::WRL::ComPtr<ID3D11Device> _device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext, IControllerPtr controller)
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
    result = ImGui_ImplDX11_Init(_device.Get(), _deviceContext.Get());
    return result;
}

void GuiView::DisplayWindows()
{
    isFlythroughOn = m_terrainController->IsFlythroughModeOn();
    isTrajectoryLoaded = m_flythroughState.IsTrajectoryInitialized;
    if (m_show_GeneralWin)
        GeneralWindow();
    if (m_show_HelpWindow)
        Help();
    if (m_show_FlythroughWin && isFlythroughOn)
        FlythroughWindow();
    if (m_show_Explore3DWin && !isFlythroughOn)
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

3. Load a configuration file or set the position of the meshes and trajectory using the graphical user interface (GUI).

4. Set the output directory path where you want to save the frames of the picture.)";
        ImGui::TextWrapped(text.c_str());

        ImGui::SeparatorText("Controls");
        if (ImGui::BeginTable("Controls", 2))
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


        if (ToggleButton("Mode", &isFlythroughOn, isTrajectoryLoaded))
        {
            if (isFlythroughOn)
                this->m_terrainController->HandleMessage(IDC_ACTIVATE_FLYTHROUGH_MODE, {}, {});
            else
                this->m_terrainController->HandleMessage(IDC_ACTIVATE_3DEXPLORE_MODE, {}, {});
        }

        if (ImGui::CollapsingHeader("Origo LLA"))
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
        if (ImGui::CollapsingHeader("Configurations"))
        {
            if (ImGui::Checkbox("Shading", &m_generalState.isShadingOn ))
            {
                m_terrainController->HandleMessage(IDC_PIXELSHADER_SET_SHADING, {  }, { m_generalState.isShadingOn });
            }
            if (ImGui::Checkbox("Grid", &m_generalState.isGridSeen))
            {
                m_terrainController->HandleMessage(IDC_XZ_PLANE_GRID_SET_ISSEEN, {  }, { m_generalState.isGridSeen });
            }
        }


        if (ImGui::CollapsingHeader("Camera Properties"))
        {
            float fov = m_generalState.fieldOfView;
            if (ImGui::SliderFloat("Field of view", &fov, 0.5, /*0.01*/ PI, "%.3f"))
            {
                this->m_terrainController->HandleMessage(IDC_SET_CAMERA_FIELD_OF_VIEW, { fov }, {});
            }
            float nearScreen = m_generalState.screenNear;
            if (ImGui::SliderFloat("NearScreen", &nearScreen, 0.5, /*0.01*/ 5, "%.3f"))
            {
                this->m_terrainController->HandleMessage(IDC_SET_CAMERA_NEAR_SCREEN, { nearScreen }, {});
            }
            float farScreen = m_generalState.screenDepth;
            if (ImGui::SliderFloat("FarScreen", &farScreen, 10, /*0.01*/ 3000, "%.3f"))
            {
                this->m_terrainController->HandleMessage(IDC_SET_CAMERA_FAR_SCREEN, { farScreen }, {});
            }

            if (ImGui::Button("Reset Camera"))
            {
                this->m_terrainController->HandleMessage(IDC_E3D_CAMERA_RESET, {}, {});
            }
        }
        if (ImGui::CollapsingHeader("Terrain Meshes"))
        {
            ImGui::SeparatorText("Scale");
            if (ImGui::SliderFloat("slider S", &m_GroupTrans.scaling, 0, 10))
            {
                m_terrainController->HandleMessage(IDC_MESH_GROUP_SCALE, { m_GroupTrans.scaling }, {  });
            }
            if (ImGui::InputFloat("input S", &m_GroupTrans.scaling))
            {
                m_terrainController->HandleMessage(IDC_MESH_GROUP_SCALE, { m_GroupTrans.scaling }, {  });
            }


            ImGui::SeparatorText("Rotation radian");
            if (ImGui::SliderFloat3("slider R", m_GroupTrans.rotation, -PI, PI))
            {
                m_terrainController->HandleMessage(IDC_MESH_GROUP_ROTATION, { m_GroupTrans.rotation[0],m_GroupTrans.rotation[1],m_GroupTrans.rotation[2] }, {});
            }

            if (ImGui::InputFloat3("input R", m_GroupTrans.rotation))
            {
                m_terrainController->HandleMessage(IDC_MESH_GROUP_ROTATION, { m_GroupTrans.rotation[0], m_GroupTrans.rotation[1], m_GroupTrans.rotation[2] }, {});
            }

            ImGui::SeparatorText("Translation");
            if (ImGui::DragFloat3("slider T", m_GroupTrans.tranlation, 0.1f))
            {
                m_terrainController->HandleMessage(IDC_MESH_GROUP_TRANSLATION, { m_GroupTrans.tranlation[0], m_GroupTrans.tranlation[1], m_GroupTrans.tranlation[2] }, {});
            }
            if (ImGui::InputFloat3("input T", m_GroupTrans.tranlation))
            {
                m_terrainController->HandleMessage(IDC_MESH_GROUP_TRANSLATION, { m_GroupTrans.tranlation[0], m_GroupTrans.tranlation[1], m_GroupTrans.tranlation[2] }, {});
            }

            if (ImGui::Button("Clear Terrain meshes"))
            {
                m_terrainController->HandleMessage(IDC_BUTTON_CLEAR_MESHES, {}, {});
            }

            TerrainListBox();
        }
        if (ImGui::CollapsingHeader("Trajectory"))
        {
            if (ImGui::Button("Clear Trajectory"))
            {
                m_terrainController->HandleMessage(IDC_BUTTON_CLEAR_TRAJECTORY, {}, {});
            }
            static int item_current_idx = 0; // Here we store our selection data as an index.

            if (!m_flythroughState.trajectoryPolyLine.empty())
            {
                TrajectoryState& trajectoryState = m_flythroughState.trajectoryPolyLine.at(0);
                std::string polyLineName = StringConverter::WideToString(trajectoryState.name);
                ImGui::PushID(polyLineName.c_str());

                static bool is_selected = false;
                if (ImGui::Selectable(polyLineName.c_str(), &is_selected))
                {
                    ImGui::OpenPopup("Trajectory");
                }
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
                TrajectoryPopUp(m_TrajectoryTrans);
                ImGui::PopID();
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
    ImGui::End();
}



std::vector<std::string> GuiView::CollectTerrainIDNames(void)
{
    std::vector<std::string> names;
   
    for (const MeshState& info : m_TerrainsState.Meshes)
    {
        std::string id = StringConverter::WideToString(info.name) + ", id: " + std::to_string(info.id);
        names.push_back(id);
    }
    return names;

}

void GuiView::TerrainPopUp(unsigned int terrainId, MeshTransformation& t)
{
    if (ImGui::BeginPopup("Terrain")) //BeginPopupContextItem())
    {
        if (ImGui::Checkbox("IsSeen", &t.m_isSeen))
        {
            float b = (float)t.m_isSeen;
            m_terrainController->HandleMessage(IDC_MESH_SET_ISSEEN, { b }, { terrainId });
        }

         ImGui::SeparatorText("Color");
         if (ImGui::ColorEdit4("color", t.color))
         {
             m_terrainController->HandleMessage(IDC_MESH_SET_COLOR, {t.color[0],t.color[1],t.color[2],t.color[3]}, {terrainId});
         }
        ImGui::EndPopup();
    }
}

void GuiView::TrajectoryPopUp(TrajectoryTransformation& t)
{
    if (ImGui::BeginPopup("Trajectory")) //BeginPopupContextItem())
    {
        
        if (ImGui::Checkbox("IsSeen", &t.m_isSeen))
        {
            float b = (float)t.m_isSeen;
            m_terrainController->HandleMessage(IDC_TRAJECTORY_SET_ISSEEN, { b }, {  });
        }
        
        ImGui::SeparatorText("Rotation radian");
        if (ImGui::SliderFloat3("slider R", t.rotation, -PI, PI))
        {
           m_terrainController->HandleMessage(IDC_TRAJECTORY_ROTATION, { t.rotation[0], t.rotation[1] ,t.rotation[2] }, {});
        }

        if (ImGui::InputFloat3("input R", t.rotation))
        {
          m_terrainController->HandleMessage(IDC_TRAJECTORY_ROTATION, { t.rotation[0], t.rotation[1] ,t.rotation[2] }, {  });
        }

        ImGui::SeparatorText("Translation");
        if (ImGui::DragFloat3("slider T", t.tranlation,0.1f))
        {
            m_terrainController->HandleMessage(IDC_TRAJECTORY_TRANSLATION, { t.tranlation[0], t.tranlation[1] ,t.tranlation[2] }, {});
        }
        if (ImGui::InputFloat3("input T", t.tranlation))
        {
            m_terrainController->HandleMessage(IDC_TRAJECTORY_TRANSLATION, { t.tranlation[0], t.tranlation[1] ,t.tranlation[2] }, {});
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
        for (int n = 0; n < terrainIds.size(); n++)
        {
            ImGui::PushID(n);
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(terrainIds.at(n).c_str(), is_selected))
            {
                item_current_idx = n;
                ImGui::OpenPopup("Terrain");
            }
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }

            unsigned int terrainId = m_TerrainsState.Meshes.at(item_current_idx).id;
            auto it = std::find_if(m_MeshElementsTrans.begin(), m_MeshElementsTrans.end(), [terrainId](MeshTransformation t) {return t.id == terrainId; });

            this->TerrainPopUp(terrainId, *it);
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
        ImGui::Text("%.4f", state.currentCameraPosition.x);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.4f", state.currentCameraPosition.y);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.4f", state.currentCameraPosition.z);
        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::SeparatorText("Rotation (radian) [pitch, yaw, roll]");
    if (ImGui::BeginTable("Rotation", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%.4f", state.currentCameraRotation.x);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.4f", state.currentCameraRotation.y);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.4f", state.currentCameraRotation.z);
        ImGui::EndTable();
    }

    ImGui::SeparatorText("Origo LLA coords");
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
    ImGui::SeparatorText("Sun Position");
    {
        if (ImGui::BeginTable("Sun Position", 2))
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
        ImGui::SeparatorText("Buttons");
        if (ImGui::Button("Play"))
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_START, {}, {});

        ImGui::SameLine();
        if (ImGui::Button("Pause"))
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_PAUSE, {}, {});

        ImGui::SameLine();
        if (ImGui::Button("Stop"))
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_STOP, {}, {});      
        
        static bool isRecording = false;  
        if (!isRecording)
        {
            if (ImGui::Button("Record Frames"))
            {
                THROW_TREXCEPTION_IF_FAILED(!m_outputDir.empty(), L"Screen capture failed because the output directory was not specified. To fix this, select the \"File > Set output directory\" menu item and choose the directory where you want to save the captured screen images.");
                this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_RECORD_START, {}, {});
                isRecording = true;
            }
        }
        else
        {
            if (ImGui::Button("Stop Record"))
            {
                isRecording = false;
                this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_RECORD_STOP, {}, {});
            }
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
        if (ImGui::InputText("UnixTime_FT", ut, 11))
        {
            unsigned newUnix = std::atol(ut);
            if (newUnix > 0)
            {
                this->m_terrainController->HandleMessage(IDC_SET_START_TIME_TRAJECTORY, {}, {newUnix});
            }
        }      
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
        if (ImGui::InputText("UnixTime_E3D", ut, 11))
        {
            unsigned newUnix = std::atol(ut);
            if (newUnix > 0)
            {
                this->m_terrainController->HandleMessage(IDC_SET_TIME_E3D, {}, { newUnix });
            }
        }

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
    m_TerrainsState = states;

    Vector3DtoCArray(m_GroupTrans.rotation, states.rotation);
    m_GroupTrans.scaling = states.scale.x;
    Vector3DtoCArray(m_GroupTrans.tranlation, states.translation);


    if (states.Meshes.empty())
    {
        m_MeshElementsTrans.clear();
    }
    for (const MeshState& state : states.Meshes)
    {
        auto it = std::find_if(m_MeshElementsTrans.begin(), m_MeshElementsTrans.end(), [state](MeshTransformation t) {return t.id == state.id; });
        //not contains
        if (it == m_MeshElementsTrans.end())
        {
            MeshTransformation tranformation;
            tranformation.id = state.id;
            Vector4DtoCArray(tranformation.color, state.color);
            tranformation.m_isSeen = state.m_isSeen;
            m_MeshElementsTrans.push_back(tranformation);
        }
        else
        {
            Vector4DtoCArray(it->color, state.color);
        }
    }
}

void GuiView::HandleIModelState(const FlythroughState& state)
{
    m_flythroughState = state;

    if (state.trajectoryPolyLine.empty())
    {
        m_TrajectoryTrans = TrajectoryTransformation();
    }

    else 
    {
        TrajectoryState trajectoryState = state.trajectoryPolyLine.at(0);
        TrajectoryTransformation tt;
        tt.id       = trajectoryState.id;
        tt.m_isSeen = trajectoryState.m_isSeen;
        Vector3DtoCArray(tt.rotation, trajectoryState.rotation);
        Vector3DtoCArray(tt.tranlation, trajectoryState.translation);
        m_TrajectoryTrans = tt;
    }

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
