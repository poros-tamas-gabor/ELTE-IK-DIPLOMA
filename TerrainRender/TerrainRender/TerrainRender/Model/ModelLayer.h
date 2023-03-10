#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Persistence/ModelStructs.h"
#include "ModelLight.h"
#include "Persistence/DataAccess.h"
#include <memory>
#include "../Graphics/IObserver.h"

class ModelLayer
{
private:
	std::vector<IObserver*>					_observers;
	ModelLight								_modelLight;
	IDataAccess*							_persistence;
	std::vector<ModelVertex>				_vertices;

public:
	ModelLayer();
	~ModelLayer();

	bool Attach(IObserver* observer);
	bool Detach(IObserver* observer);
	void NotifyObservers(void);

	bool Initalize(IDataAccess* persistence);
	bool LoadTerrain(const wchar_t* filepath);
	void Shutdown();
	const std::vector<ModelVertex>& GetVertices( void ) const;

	void GetLight(const ModelLight** modelLight) const;
};
#endif // !MODEL_H

