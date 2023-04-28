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
    ImGui::PushItemWidth(ImGui::GetFontSize() * -15);
    ImGui::Begin("General Settings Window", &m_show_GeneralWin, 0);


    if (ToggleButton("Mode", &isFlythroughOn, isTrajectoryLoaded))
    {
        if (isFlythroughOn)
            this->m_terrainController->HandleMessage(IDC_ACTIVATE_FLYTHROUGH_MODE, {}, {});
        else
            this->m_terrainController->HandleMessage(IDC_ACTIVATE_3DEXPLORE_MODE, {}, {});
    }
    if (ImGui::CollapsingHeader("Camera Properties"))
    {
        float fov = m_cameraState.fieldOfView;
        if (ImGui::SliderFloat("Field of view", &fov, 0.5, /*0.01*/ PI, "%.3f"))
        {
            this->m_terrainController->HandleMessage(IDC_SLIDER_PROJECTION_FIELD_OF_VIEW, { fov }, {});
        }
        float nearScreen = m_cameraState.screenNear;
        if (ImGui::SliderFloat("NearScreen", &nearScreen, 0.5, /*0.01*/ 5, "%.3f"))
        {
            this->m_terrainController->HandleMessage(IDC_SLIDER_PROJECTION_NEAR_SCREEN, { nearScreen }, {});
        }
        float farScreen = m_cameraState.screenDepth;
        if (ImGui::SliderFloat("FarScreen", &farScreen, 10, /*0.01*/ 3000, "%.3f"))
        {
            this->m_terrainController->HandleMessage(IDC_SLIDER_PROJECTION_FAR_SCREEN, { farScreen }, {});
        }

        if (ImGui::Button("Reset Camera"))
        {
            this->m_terrainController->HandleMessage(IDC_E3D_CAMERA_RESET, {}, {});
        }
    }

    if (ImGui::CollapsingHeader("Terrain Meshes"))
    {
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

        std::vector<std::string> polyLineId;
        for (IRenderableState state : m_flythroughState.trajectoryPolyLine)
        {
            polyLineId.push_back("polyline: " + StringConverter::WideToString(state.name));
        }
        for (int n = 0; n < polyLineId.size(); n++)
        {
            ImGui::PushID(polyLineId.at(n).c_str());
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(polyLineId.at(n).c_str(), is_selected))
            {
                item_current_idx = n;
                ImGui::OpenPopup("Terrain");
            }
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }

            unsigned int polylineId = m_flythroughState.trajectoryPolyLine.at(n).id;


            IRenderablePopUp(polylineId, RenderableTypes::TrajectoryPolyline, m_TrajectoryTransformation.at(n));
            ImGui::PopID();
        }
    }
    ImGui::End();
}



std::vector<std::string> GuiView::CollectTerrainIDNames(void)
{
    std::vector<std::string> names;
    for (const IRenderableState& info : m_TerrainsState)
    {
        std::string id = "Mesh: " + StringConverter::WideToString(info.name) + " id: " + std::to_string(info.id);
        names.push_back(id);
    }
    return names;

}

void GuiView::IRenderablePopUp(unsigned int terrainId, RenderableTypes type, IRenderableTransformation& t)
{
    if (ImGui::BeginPopup("Terrain")) //BeginPopupContextItem())
    {
        
        if (ImGui::Checkbox("IsSeen", &t.m_isSeen))
        {
            float b = (float)t.m_isSeen;
            m_terrainController->HandleMessage(IDC_CHECKBOX_IRENDERABLE_ISSEEN, { b }, { terrainId });
        }
        
        if (type == RenderableTypes::Terrain)
        {
            ImGui::SeparatorText("Scale");
            if (ImGui::SliderFloat("slider S", &t.scaling, 0, 10))
            {
                m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_SCALE, { t.scaling }, { terrainId });
            }
            if (ImGui::InputFloat("input S", &t.scaling))
            {
                m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_SCALE, { t.scaling }, { terrainId });
            }
        }

        ImGui::SeparatorText("Rotation radian");
        if (ImGui::SliderFloat3("slider R", t.rotation, -PI, PI))
        {
           // m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_ROTATION, { t.rotation }, { terrainId });
        }

        if (ImGui::InputFloat3("input R", t.rotation))
        {
           // m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_ROTATION, t.rotation, &terrainId);
        }

        ImGui::SeparatorText("Translation");
        if (ImGui::DragFloat3("slider T", t.tranlation,0.1f))
        {
           // m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_TRANSLATION, t.tranlation, &terrainId);
        }
        if (ImGui::InputFloat3("input T", t.tranlation))
        {
           // m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_TRANSLATION, t.tranlation, &terrainId);
        }

        if (type == RenderableTypes::Terrain)
        {
            ImGui::SeparatorText("Color");
            if (ImGui::ColorEdit4("color", t.color))
            {
                //m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_COLOR, t.color, &terrainId);
            }
        }

        ImGui::EndPopup();
    }
}

void GuiView::TerrainListBox()
{
    static int item_current_idx = 0; // Here we store our selection data as an index.
    if (ImGui::BeginListBox(""))
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

            unsigned int terrainId = m_TerrainsState.at(item_current_idx).id;
            auto it = std::find_if(m_TerrainTrasnformations.begin(), m_TerrainTrasnformations.end(), [terrainId](IRenderableTransformation t) {return t.id == terrainId; });

            IRenderablePopUp(terrainId, RenderableTypes::Terrain, *it);
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
    ImGui::PushItemWidth(ImGui::GetFontSize() * -15);
    ImGui::Begin("General Flythrough Window", &m_show_FlythroughWin, 0);
    try
    {
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
            if (ImGui::Button("Record"))
            {
                THROW_TREXCEPTION_IF_FAILED(!m_outputDir.empty(), L"Failed to capture screen because the output directory was not choose");
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
        static float flythrough_speed = 1;
        if (ImGui::SliderFloat("Speed", &flythrough_speed, 0.1f, 3.0f, "%.3f"))
        {
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_SET_SPEED, { flythrough_speed }, {});
        }
        
        m_frame = m_flythroughState.currentFrame;
        ImGui::Text("Frame: %d / %d", m_frame, m_flythroughState.numberOfFrame);
        if (ImGui::SliderInt("Frames", &m_frame, 0, max(0,m_flythroughState.numberOfFrame-1)))
        {
            this->m_terrainController->HandleMessage(IDC_FLYTHROUGH_SET_FRAME, {}, { (unsigned)(m_frame) });
        }
        std::string unixtimestr = std::to_string(m_flythroughState.startEpochTime.getSeconds());
        char ut[11];
        strcpy_s<11>(ut, unixtimestr.c_str());
        if (ImGui::InputText("UnixTime_FT", ut, 11))
        {
            unsigned newUnix = std::atol(ut);
            if (newUnix > 0)
            {
                this->m_terrainController->HandleMessage(IDC_INPUT_FLYTHROUGH_UNIXTIME, {}, {newUnix});
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
    ImGui::End();
}
void GuiView::Explore3DWindow()
{
    ImGui::PushItemWidth(ImGui::GetFontSize() * -15);
    ImGui::Begin("Explore 3D Window", &m_show_Explore3DWin, 0);

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
    if (ImGui::InputText("UnixTime_E3D",ut, 11))
    {
        unsigned newUnix = std::atol(ut);
        if (newUnix > 0)
        {
            this->m_terrainController->HandleMessage(IDC_INPUT_3DE_UNIXTIME, {}, { newUnix });
        }
    }

    PrintStatus<Explore3DState>(m_explore3dState);
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




void GuiView::HandleIModelState(const std::vector<IRenderableState>& states)
{
    m_TerrainsState = states;
    if (states.empty())
    {
        m_TerrainTrasnformations.clear();
    }
    for (const IRenderableState& state : states)
    {
        auto it = std::find_if(m_TerrainTrasnformations.begin(), m_TerrainTrasnformations.end(), [state](IRenderableTransformation t) {return t.id == state.id; });
        //not contains
        if (it == m_TerrainTrasnformations.end())
        {
            //TODO lehet hogy itt initializalni kell 
            IRenderableTransformation tranformation;
            tranformation.id = state.id;
            Vector3DtoCArray(tranformation.rotation, state.rotation);
            Vector3DtoCArray(tranformation.tranlation, state.translation);
            tranformation.scaling = state.scale.x;
            Vector4DtoCArray(tranformation.color, state.color);
            tranformation.m_isSeen = state.m_isSeen;

            m_TerrainTrasnformations.push_back(tranformation);
        }
        else
        {
            Vector4DtoCArray(it->color, state.color);
            Vector3DtoCArray(it->rotation, state.rotation);
            Vector3DtoCArray(it->tranlation, state.translation);
        }
    }
}

void GuiView::HandleIModelState(const FlythroughState& state)
{
    m_flythroughState = state;

    if (state.trajectoryPolyLine.empty())
    {
        m_TrajectoryTransformation.clear();
    }

    for (const IRenderableState& state : state.trajectoryPolyLine)
    {
        auto it = std::find_if(m_TrajectoryTransformation.begin(), m_TrajectoryTransformation.end(), [state](IRenderableTransformation t) {return t.id == state.id; });
        //not contains
        if (it == m_TrajectoryTransformation.end())
        {
            //TODO lehet hogy itt initializalni kell 
            IRenderableTransformation tranformation;
            tranformation.id = state.id;
            Vector3DtoCArray(tranformation.rotation, state.rotation);
            Vector3DtoCArray(tranformation.tranlation, state.translation);
            tranformation.scaling = state.scale.x;
            Vector4DtoCArray(tranformation.color, state.color);
            tranformation.m_isSeen = state.m_isSeen;

            m_TrajectoryTransformation.push_back(tranformation);
        }
        else
        {
            Vector4DtoCArray(it->color, state.color);
            Vector3DtoCArray(it->rotation, state.rotation);
            Vector3DtoCArray(it->tranlation, state.translation);
        }
    }
    m_TrajectoryTransformation;
}

void GuiView::HandleIModelState(const Explore3DState& state)
{
    m_explore3dState = state;
}
void GuiView::HandleIModelState(const CameraState& state)
{
    m_cameraState = state;
}

void GuiView::SetOutputDirectory(const std::wstring& dir)
{
    m_outputDir = dir;
}
