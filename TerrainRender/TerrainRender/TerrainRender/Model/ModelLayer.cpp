#include "ModelLayer.h"
#include <algorithm>


ModelLayer::ModelLayer() = default;

ModelLayer::~ModelLayer() = default;

void ModelLayer::Shutdown() {}

bool ModelLayer::Initalize(IDataAccess* persistence) 
{
	_persistence = persistence;

	_modelLight.SetDiffuseColor(0.4f, 0.4f, 0.4f, 1.0f);
	_modelLight.SetDirection(-1.0f, -1.0f, -1.0f, 0.0f);

	return (_persistence != nullptr);
}

bool ModelLayer::LoadTerrain(const wchar_t* filepath)
{
	bool bresult = _persistence->LoadTerrain(filepath, this->_vertices);
	this->NotifyObservers();
	return bresult;

}
const std::vector<ModelVertex>& ModelLayer::GetVertices(void) const
{
	return this->_vertices;
}


bool ModelLayer::Attach(IObserver* observer)
{
	if (std::find(this->_observers.begin(), this->_observers.end(), observer) != this->_observers.end())
	{
		return false;
	}
	else {
		this->_observers.push_back(observer);
		return false;
	}
}
bool ModelLayer::Detach(IObserver* observer)
{
	if (std::find(this->_observers.begin(), this->_observers.end(), observer) != this->_observers.end())
	{
		std::remove(this->_observers.begin(), this->_observers.end(), observer);
		return true;
	}
	return false;
}
void ModelLayer::NotifyObservers(void)
{
	for (IObserver* o : _observers)
	{
		o->Update();
	}
}

void ModelLayer::GetLight(const ModelLight** modelLight) const
{
	*modelLight = &this->_modelLight;
}

