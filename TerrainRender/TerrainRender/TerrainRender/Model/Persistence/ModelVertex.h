#ifndef MODEL_VERTEX_H
#define MODEL_VERTEX_H


struct ModelVector3D
{
	float x;
	float y;
	float z;
};

struct ModelVector4D
{
	float x;
	float y;
	float z;
	float w;
};

struct
	ModelVertex
{
	ModelVector3D position;
	ModelVector3D normal;
	ModelVector3D color = {0,0,0};

	void Clear()
	{
		position = { 0,0,0 };
		normal = { 0,0,0 };
		color = { 0,0,0 };
	}
};
#endif // !MODEL_VERTEX_H
