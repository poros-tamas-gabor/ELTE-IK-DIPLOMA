#ifndef I_VIEW_H
#define I_VIEW_H

#include <memory>
#include <vector>
#include "../Model/Persistence/ModelStructs.h"
class IView
{
public:
	virtual ~IView() {}
	virtual void HandleIRenderableInfo(const std::vector<IRenderableInformation>&) = 0;
	//virtual void Update(const ModelEvent::Event& event) = 0;
};

typedef std::shared_ptr<IView> IViewPtr;
#endif