#ifndef MODEL_STRUCTS_H
#define MODEL_STRUCTS_H

#include "EpochTime.h"
#include <DirectXMath.h>
#include <string>
#include <vector>


struct VertexMesh
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
};

struct VertexPolyLine
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

struct ModelTime
{
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
};

struct ModelLocation
{
	double lat;
	double longitude;
};

struct CameraPose {

	EpochTime epochtime;

	double yaw;
	double pitch;
	double roll;

	double north;
	double east;
	double down;
};

struct Vector3D
{
public:
	double x;
	double y;
	double z;

	Vector3D operator*(double factor) const
	{
		return { this->x * factor, this->y * factor, this->z * factor};
	}

	Vector3D operator+(const Vector3D& other) const
	{
		return { this->x + other.x, this->y + other.y, this->z + other.z };
	}
	Vector3D operator-(const Vector3D& other) const
	{
		return { this->x - other.x, this->y - other.y, this->z - other.z };
	}
};

inline Vector3D operator*(double factor, const Vector3D& other)
{
	return other * factor;
}

struct Vector4D
{
	float x, y, z, w;
};



struct Facet
{
	float normal[3];
	float position[3][3];
};

enum STLLineType { BEGIN, FACET, LOOP, VERTEX, ENDLOOP, ENDFACET, END };


struct stlFile
{
	uint8_t     header[80];
	uint32_t    numOfTriangles;
	std::vector<Facet> facets;
};

struct IRenderableState
{
	unsigned			id;
	std::wstring		name;
	DirectX::XMMATRIX	localMatrix;
	DirectX::XMFLOAT3	rotation;
	DirectX::XMFLOAT3	scale;
	DirectX::XMFLOAT3	translation;
};

struct SunPositionState
{
	double azimuth = 0;
	double elevation = 0;
};
struct FlythroughState
{
	bool				IsTrajectoryLoaded	= false;
	unsigned			currentFrame		= 0;
	unsigned			numberOfFrame		= 0;
	EpochTime			currentEpochTime	= { 0,0 };
	DirectX::XMFLOAT3	currentPosition		= { 0,0,0 };
	DirectX::XMFLOAT3	currentRotation		= { 0,0,0 };
	SunPositionState	currentSunPosition;
};

struct Explore3DState
{
	EpochTime			currentEpochTime	= { 0,0 };
	DirectX::XMFLOAT3	currentPosition		= { 0,0,0 };
	DirectX::XMFLOAT3	currentRotation		= { 0,0,0 };
	SunPositionState	currentSunPosition;
};


#endif // !MODEL_VERTEX_H
