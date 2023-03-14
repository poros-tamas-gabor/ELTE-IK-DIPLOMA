#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Persistence/ModelStructs.h"
#include "ModelLight.h"
#include "Persistence/DataAccess.h"
#include <memory>
#include "../Graphics/IObserver.h"
#include "ModelEvent.h"

class ModelLayer
{
private:
	std::vector<IObserver*>					_observers;
	ModelLight								_modelLight;
	IDataAccess*							_persistence;
	std::vector<ModelVertex>				_vertices;
	bool	_isTerrainLoaded;
	bool	_isTrajektoryLoaded;

public:
	ModelLayer();
	~ModelLayer();


	bool IsTrajektoryLoaded(void);
	bool IsTerrainLoaded(void);
	bool Attach(IObserver* observer);
	bool Detach(IObserver* observer);
	void NotifyObservers(const ModelEvent::Event& event);

	bool Initalize(IDataAccess* persistence);
	bool LoadTerrain(const wchar_t* filepath);
	bool LoadCameraTrajectory(const wchar_t* filepath);
	void Shutdown();
	const std::vector<ModelVertex>& GetVertices( void ) const;

	void GetpModelLight(const ModelLight** modelLight) const;
};
#endif // !MODEL_H

