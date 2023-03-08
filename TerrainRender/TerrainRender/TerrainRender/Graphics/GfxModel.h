#ifndef	GFX_MODEL_H
#define	GFX_MODEL_H

#include "GfxVertex.h"
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

class ModelLayer;
class GfxModel
{
private:

	struct GfxVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 color;
	};

private:
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	unsigned int	_vertexCount;
	unsigned int	_indexCount;


public:
	GfxModel() = default;
	~GfxModel() = default;
	GfxModel(const GfxModel&) = delete;
	GfxModel& operator=(const GfxModel&) = delete;

	
	bool Initialize(ID3D11Device*, const ModelLayer* );
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount() const;
	int GetVertexCount() const;

private:
	bool InitializeBuffers(ID3D11Device*, const ModelLayer*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);



};



#endif