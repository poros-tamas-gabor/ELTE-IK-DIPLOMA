#include "GuiView.h"
#include <commdlg.h>
#include "../resource.h"
#include "../ImGui/imguistyleserializer.h" 
#include "../StringConverter.h"
#include "../ImGui/imgui_internal.h"
#include <iomanip>
#include <ctime>


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

const static float PI = 3.14159265358979323846;
bool GuiView::Initalize(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, IControllerPtr controller)
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
    ImGui::PushItemWidth(ImGui::GetFontSize() * -15);
    ImGui::Begin("Setting Window", &show_setting_window, ImGuiWindowFlags_MenuBar ); 
    
    MenuBar();
    static bool isFlythroughOn = false;
    bool isTrajectoryLoaded = m_flythroughState.IsTrajectoryInitialized;
    if (ToggleButton("Mode", &isFlythroughOn, isTrajectoryLoaded))
    {
        if(isFlythroughOn)
            this->m_terrainController->HandleMessage(IDC_BUTTON_FLYTHROUGH_MODE, NULL, NULL);
        else
            this->m_terrainController->HandleMessage(IDC_BUTTON_3DEXPLORE_MODE, NULL, NULL);
    }


    ImGui::Text("isFlyModeon: %d", isFlythroughOn);
    if (ImGui::BeginTabBar("TabBar"))
    {
        if (ImGui::BeginTabItem("General"))
        {
            GeneralTab();
            ImGui::EndTabItem();
        }
        if (!isFlythroughOn)
        {
            if (ImGui::BeginTabItem("Explore 3D"))
            {
                Explore3DTab();
                ImGui::EndTabItem();
            }
        }
        if (isFlythroughOn)
        {
            if (ImGui::BeginTabItem("Flythrough"))
            {
                FlythroughTab();
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}


void GuiView::MenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open Terrain File"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FIlE_TERRAIN, NULL, NULL);
            }

            if (ImGui::MenuItem("Open Terrain Project"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FIlE_TERRAIN_PROJECT, NULL, NULL);
            }

            if (ImGui::MenuItem("Open Camera Trajectory"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FIlE_CAMERA_TRAJECTORY, NULL, NULL);
            }

            if (ImGui::MenuItem("Open Parameter File"))
            {
                this->m_terrainController->HandleMessage(IDC_BUTTON_FIlE_PARAMETERS, NULL, NULL);
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
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

void GuiView::IRenderablePopUp(unsigned int terrainId, IRenderableTransformation& t)
{
    if (ImGui::BeginPopup("Terrain")) //BeginPopupContextItem())
    {

        if (ImGui::Checkbox("IsSeen", &t.m_isSeen))
        {
            float b = (float)t.m_isSeen;
            m_terrainController->HandleMessage(IDC_CHECKBOX_IRENDERABLE_ISSEEN, &b, &terrainId);
        }
        
        ImGui::SeparatorText("Scale");
        if (ImGui::SliderFloat("slider S", &t.scaling, 0, 10))
        {
            m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_SCALE, &t.scaling, &terrainId);
        }

        if (ImGui::InputFloat("input S", &t.scaling))
        {
            m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_SCALE, &t.scaling, &terrainId);
        }

        ImGui::SeparatorText("Rotation radian");
        if (ImGui::SliderFloat3("slider R", t.rotation, -PI, PI))
        {
            m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_ROTATION, t.rotation, &terrainId);
        }

        if (ImGui::InputFloat3("input R", t.rotation))
        {
            m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_ROTATION, t.rotation, &terrainId);
        }

        ImGui::SeparatorText("Translation");
        if (ImGui::DragFloat3("slider T", t.tranlation,0.1))
        {
            m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_TRANSLATION, t.tranlation, &terrainId);
        }
        if (ImGui::InputFloat3("input T", t.tranlation))
        {
            m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_TRANSLATION, t.tranlation, &terrainId);
        }

        ImGui::SeparatorText("Color");
        if (ImGui::ColorEdit4("color", t.color))
        {
            m_terrainController->HandleMessage(IDC_SLIDER_IRENDERABLE_COLOR, t.color, &terrainId);
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

            IRenderablePopUp(terrainId, *it);
            ImGui::PopID();
        }
        ImGui::EndListBox();
    }
}
void GuiView::GeneralTab()
{
    if (ImGui::CollapsingHeader("Camera Properties"))
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

        if (ImGui::Button("Reset Camera"))
        {
            this->m_terrainController->HandleMessage(IDC_BUTTON_CAMERA_RESET, NULL, NULL);
        }
    }

    if (ImGui::CollapsingHeader("Terrain Meshes"))
    {
        if (ImGui::Button("Clear Terrain meshes"))
        {
            m_terrainController->HandleMessage(IDC_BUTTON_CLEAR_MESHES, NULL, NULL);
        }

        TerrainListBox();
    }
    if (ImGui::CollapsingHeader("Trajectory"))
    {
        if (ImGui::Button("Clear Trajectory"))
        {
            m_terrainController->HandleMessage(IDC_BUTTON_CLEAR_TRAJECTORY, NULL, NULL);
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


                IRenderablePopUp(polylineId,m_TrajectoryTransformation.at(n));
                ImGui::PopID();
            }
    }
}
void GuiView::FlythroughTab()
{
    ImGui::SeparatorText("Buttons");
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

    ImGui::SeparatorText("Properties");
    static float flythrough_speed = 1;
    if (ImGui::SliderFloat("Speed", &flythrough_speed, 0.1, 3, "%.3f"))
    {
        this->m_terrainController->HandleMessage(IDC_SLIDER_FLYTHROUGH_SPEED, &flythrough_speed, NULL);
    }

    m_frame = m_flythroughState.currentFrame;
    ImGui::Text("Frame: %d / %d", m_frame, m_flythroughState.numberOfFrame);
    if (ImGui::SliderInt("Frames", &m_frame, 0, m_flythroughState.numberOfFrame))
    {
        this->m_terrainController->HandleMessage(IDCC_SET_FRAME_FLYTHROUGH, NULL, (unsigned*)(&m_frame));
    }

    ImGui::Spacing();
    ImGui::SeparatorText("Position");
    if (ImGui::BeginTable("Position", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%.4f", m_flythroughState.currentCameraPosition.x);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.4f", m_flythroughState.currentCameraPosition.y);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.4f", m_flythroughState.currentCameraPosition.z);
        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::SeparatorText("Rotation (radian) [pitch, yaw, roll]");
    if (ImGui::BeginTable("Rotation", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%.4f", m_flythroughState.currentCameraRotation.x);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.4f", m_flythroughState.currentCameraRotation.y);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.4f", m_flythroughState.currentCameraRotation.z);
        ImGui::EndTable();
    }
    ImGui::SeparatorText("Date and time");
    std::time_t time(m_flythroughState.currentEpochTime.getSeconds());
    std::tm tm;
    gmtime_s(&tm, &time);
    char buffer[80];
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &tm);
    ImGui::Text("UTC: %s", buffer);
}
void GuiView::Explore3DTab()
{
    ImGui::SeparatorText("FPS camera properties");
    static float cameraSpeed = 0.005f;
    if (ImGui::SliderFloat("speed", &cameraSpeed, 0, /*0.01*/ 1, "%.3f"))
    {
        this->m_terrainController->HandleMessage(IDC_SLIDER_CAMERA_SPEED, &cameraSpeed, NULL);
    }
    static float cameraRotationSpeed = 0.001f;
    if (ImGui::SliderFloat("rotation speed", &cameraRotationSpeed, 0, 0.002, "%.3f"))
    {
        this->m_terrainController->HandleMessage(IDC_SLIDER_CAMERA_ROTATION_SPEED, &cameraRotationSpeed, NULL);
    }

    ImGui::Spacing();
    ImGui::SeparatorText("Position");
    if (ImGui::BeginTable("Position", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%.4f", m_explore3dState.currentCameraPosition.x);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.4f", m_explore3dState.currentCameraPosition.y);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.4f", m_explore3dState.currentCameraPosition.z);
        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::SeparatorText("Rotation (radian) [pitch, yaw, roll]");
    if (ImGui::BeginTable("Rotation", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%.4f", m_explore3dState.currentCameraRotation.x);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.4f", m_explore3dState.currentCameraRotation.y);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.4f", m_explore3dState.currentCameraRotation.z);
        ImGui::EndTable();
    }
    ImGui::SeparatorText("Date and time");
    std::time_t time(m_explore3dState.currentEpochTime.getSeconds());
    std::tm tm;
    gmtime_s(&tm, &time);
    char buffer[80];
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &tm);
    ImGui::Text("UTC: %s", buffer);

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
            XMFLOAT3toCArray(tranformation.rotation, state.rotation);
            XMFLOAT3toCArray(tranformation.tranlation, state.translation);
            tranformation.scaling = state.scale.x;
            XMFLOAT4toCArray(tranformation.color, state.color);
            tranformation.m_isSeen = state.m_isSeen;

            m_TerrainTrasnformations.push_back(tranformation);
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
            XMFLOAT3toCArray(tranformation.rotation, state.rotation);
            XMFLOAT3toCArray(tranformation.tranlation, state.translation);
            tranformation.scaling = state.scale.x;
            XMFLOAT4toCArray(tranformation.color, state.color);
            tranformation.m_isSeen = state.m_isSeen;

            m_TrajectoryTransformation.push_back(tranformation);
        }
    }
    m_TrajectoryTransformation;
}

void GuiView::HandleIModelState(const Explore3DState& state)
{
    m_explore3dState = state;
}
