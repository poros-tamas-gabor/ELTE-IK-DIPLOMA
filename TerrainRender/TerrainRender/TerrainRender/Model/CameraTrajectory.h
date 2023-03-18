#ifndef CAMERA_TRAJECTORY_H
#define CAMERA_TRAJECTORY_H

#include "Polygon.h"
#include "Persistence/CameraPose.h"

#include <vector>
class CameraTrajectory : public PolygonLine
{
public:
	bool Initialize(ID3D11Device* device, IVertexShader* vertexShader, IPixelShader* pixelShader, const std::vector<CameraPose>& cameraPoses);
private:
	bool Initialize(ID3D11Device* device, IVertexShader*, IPixelShader*, VertexPolygon* vertices, UINT indexCount);
	void RenderBuffers(ID3D11DeviceContext*) override;
};
#endif // !CAMERA_TRAJECTORY_H
