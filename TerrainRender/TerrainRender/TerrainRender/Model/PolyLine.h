#ifndef POLYLINE_H
#define POLYLINE_H

#include "LineList.h"

class PolyLine : public LineList
{
public:
	PolyLine() = default;
	virtual ~PolyLine() = default;
	PolyLine(const PolyLine&) = delete;
	PolyLine& operator=(const PolyLine&) = delete;

protected:
	virtual void RenderBuffers(ID3D11DeviceContext*) override;
};
#endif // !POLYLINE_H
