#ifndef I_RENDERABLE_CREATOR_H
#define	I_RENDERABLE_CREATOR_H

#include "IRenderable.h"

template <class V>
class IRenderableCreator 
{
public:
	virtual ~IRenderableCreator() = default;
	virtual IRenderable<V>* CreateRenderable() const = 0;
};
#endif
