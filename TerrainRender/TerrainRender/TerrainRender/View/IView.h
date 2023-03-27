#ifndef I_VIEW_H
#define I_VIEW_H

#include <memory>
#include <vector>
#include "../Model/Persistence/ModelStructs.h"
class IView
{

public:
	virtual ~IView() {}
	virtual void HandleIModelState(const std::vector<IRenderableState>&) = 0;
	virtual void HandleIModelState(const FlythroughState&) = 0;
	virtual void HandleIModelState(const Explore3DState&) = 0;
};

typedef std::shared_ptr<IView> IViewPtr;
#endif