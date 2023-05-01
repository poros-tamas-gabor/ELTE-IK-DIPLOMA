#include "pch.h"

class TerrainModelSubscriber : public IModelSubscriber
{
public:
	MeshGroupState		m_TerrainsState;
	FlythroughState		m_flythroughState;
	Explore3DState		m_explore3dState;
	GeneralModelState	m_generalState;

	void HandleIModelState(const MeshGroupState&) override;
	void HandleIModelState(const FlythroughState&) override;
	void HandleIModelState(const Explore3DState&) override;
	void HandleIModelState(const GeneralModelState&) override;
};