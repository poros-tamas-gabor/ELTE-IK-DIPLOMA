#ifndef POLYLINE_H
#define POLYLINE_H

///////////////////////////////////////////////////////////////////////////////
// PolyLine.h
// ==========
//
// The LineList class is a derived class from the LineList.
// This class is designed to represent a polyline, which is a continuous list of lines.
// 
// By inheriting from the IRenderable interface, the PolyLine class provides the necessary methods to render the polyline to the screen.
// 
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "LineList.h"

class PolyLine : public LineList
{
public:
	PolyLine() = default;
	virtual ~PolyLine() = default;
	PolyLine(const PolyLine&) = delete;
	PolyLine& operator=(const PolyLine&) = delete;

protected:
	virtual void RenderBuffers(Microsoft::WRL::ComPtr<ID3D11DeviceContext>) override;
};
#endif // !POLYLINE_H
