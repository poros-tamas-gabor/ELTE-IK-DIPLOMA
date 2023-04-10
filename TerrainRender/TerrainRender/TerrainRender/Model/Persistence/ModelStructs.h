#ifndef MODEL_STRUCTS_H
#define MODEL_STRUCTS_H

#include "EpochTime.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <math.h>

inline void XMFLOAT3toCArray(float array[], DirectX::XMFLOAT3 floats)
{
	array[0] = floats.x;
	array[1] = floats.y;
	array[2] = floats.z;
}

inline void XMFLOAT4toCArray(float array[], DirectX::XMFLOAT4 floats)
{
	array[0] = floats.x;
	array[1] = floats.y;
	array[2] = floats.z;
	array[3] = floats.w;
}

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

struct LLACoordinate
{
	double latitude;
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
	
	void normalize()
	{
		double length = std::sqrt(x * x + y * y + z * z);
		if (length > 0)
		{
			x /= length;
			y /= length;
			z /= length;
		}
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

struct FacetIndices
{
	size_t corner[3];
};


struct VertexNormals
{
	//Vector3D meanNormal;
	std::vector<Vector3D> normals;
	size_t vertIndex;

	Vector3D sumNormals()
	{
		Vector3D sol = { 0, 0, 0 };
		for (Vector3D v : normals)
		{
			sol = sol + v;
			sol.normalize();
		}
		return sol;
	}
};
struct StlVertex
{
	Vector3D normal;
	Vector3D pos;
};


struct VertexHTindex
{
	struct Hash
	{
		// jacked up derivation of bernstiens string hasher
		std::size_t operator()(VertexHTindex const& key) const
		{
			std::size_t hash = 5381u;
			for (auto c : key.s1)
				hash = (hash << 5) + hash + c;
			for (auto c : key.s2)
				hash = (hash << 5) + hash + c;
			for (auto c : key.s3)
				hash = (hash << 5) + hash + c;
			return hash;
		}
	};
	std::string s1, s2, s3;
	// equivalence determination.
	bool operator==(VertexHTindex const& key) const
	{
		return std::tie(s1, s2, s3) == std::tie(key.s1, key.s2, key.s3);
	}
};

struct Facet
{
	float normal[3];
	float position[3][3];
};


struct stlFile
{
	uint8_t     header[80];
	uint32_t    numOfTriangles;
	std::vector<Facet> facets;
};

struct IRenderableState
{
	unsigned			id			= 0;
	std::wstring		name		= L"";
	bool				m_isSeen		= false;
	DirectX::XMFLOAT3	rotation	= {0,0,0};
	DirectX::XMFLOAT3	scale		= { 0,0,0 };
	DirectX::XMFLOAT3	translation = { 0,0,0 };
	DirectX::XMFLOAT4	color		= { 0,0,0,0 };
};

struct SunPositionState
{
	double azimuth = 0;
	double elevation = 0;
};
struct FlythroughState
{
	bool				IsTrajectoryInitialized			= false;
	unsigned			currentFrame				= 0;
	unsigned			numberOfFrame				= 0;
	EpochTime			currentEpochTime			= { 0,0 };
	DirectX::XMFLOAT3	currentCameraPosition		= { 0,0,0 };
	DirectX::XMFLOAT3	currentCameraRotation		= { 0,0,0 };
	SunPositionState	currentSunPosition;
	LLACoordinate		origo;
	std::vector<IRenderableState>	trajectoryPolyLine;
};

struct Explore3DState
{
	EpochTime			currentEpochTime			= { 0,0 };
	DirectX::XMFLOAT3	currentCameraPosition		= { 0,0,0 };
	DirectX::XMFLOAT3	currentCameraRotation		= { 0,0,0 };
	LLACoordinate		origo;
	SunPositionState	currentSunPosition;
};

struct ResizedWindowState
{
	unsigned screenWidth;
	unsigned screenHeight;
};

struct ParameterFile {

	LLACoordinate origo;
	struct Terrain {
		Vector3D translation;
		Vector3D rotation;
		std::map<std::string, Vector4D>  colors;
	} terrain;
	struct Trajectory {
		Vector3D translation;
		Vector3D rotation;
	} trajectory;
};


#endif // !MODEL_VERTEX_H
