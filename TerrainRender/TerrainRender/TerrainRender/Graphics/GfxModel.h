#ifndef	GFX_MODEL_H
#define	GFX_MODEL_H

#include "GfxVertex.h"
#include <d3d11.h>
class ModelLayer;
class GfxModel
{
public:
	//GfxModel();
	//~GfxModel();
	
	bool Initialize(ID3D11Device*, const ModelLayer* model);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	int GetVertexCount();

private:
	bool InitializeBuffers(ID3D11Device*, const ModelLayer* model);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);


private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
};



#endif