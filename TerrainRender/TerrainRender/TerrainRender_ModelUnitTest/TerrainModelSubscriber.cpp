#include "TerrainModelSubscriber.h"

void TerrainModelSubscriber::HandleIModelState(const MeshGroupState& states)
{
    m_TerrainsState = states;
}

void TerrainModelSubscriber::HandleIModelState(const FlythroughState& state)
{
    m_flythroughState = state;
}

void TerrainModelSubscriber::HandleIModelState(const Explore3DState& state)
{
    m_explore3dState = state;
}
void TerrainModelSubscriber::HandleIModelState(const GeneralModelState& state)
{
    m_generalState = state;
}