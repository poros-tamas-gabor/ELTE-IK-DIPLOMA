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
#include <sstream>
#include <array>


inline std::string to_string_with_precision(const float a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return std::move(out).str();
}

inline float to_float_with_precision(const float value, const int n = 6)
{
	return std::roundf(value * std::pow(10, n)) / std::pow(10, n);
}
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

	Vector3D (const std::array<float, 3> array) : x(array.at(0)), y(array.at(1)), z(array.at(2)) {}
	Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
	Vector3D(void) : x(0), y(0), z(0) {}

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


// For soft edges
struct CornerIndices
{
	size_t corner[3];
};

typedef std::vector<CornerIndices> IndicesVec;
typedef std::shared_ptr<IndicesVec> IndicesVecPtr;


struct NormalsInSamePositions
{
	//Vector3D meanNormal;
	std::vector<Vector3D> normals;
	size_t vertIndex;

	Vector3D sumNormals()
	{
		Vector3D sol = { 0, 0, 0 };
		for (const Vector3D& v : normals)
		{
			sol = sol + v;
			sol.normalize();
		}
		return sol;
	}
};
struct StlVertex
{
	Vector3D pos;
	Vector3D normal;
};

// taken from http://stackoverflow.com/questions/6899392/generic-hash-function-for-all-stl-containers
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct HTindex_Soft
{
	std::array<float, 3> positions;

	struct Hash
	{
		std::size_t operator()(HTindex_Soft const& key) const
		{
			size_t h = std::hash<float>()(key.positions[0]);
			hash_combine(h, key.positions[1]);
			hash_combine(h, key.positions[2]);
			return h;
		}
	};
	

	// equivalence determination.
	bool operator==(HTindex_Soft const& key) const
	{
		return positions == key.positions;
	}
};

typedef std::unordered_map<HTindex_Soft, NormalsInSamePositions, HTindex_Soft::Hash> HashTable_Soft;

struct stlFacet
{
	float normal[3];
	float position[3][3];
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
