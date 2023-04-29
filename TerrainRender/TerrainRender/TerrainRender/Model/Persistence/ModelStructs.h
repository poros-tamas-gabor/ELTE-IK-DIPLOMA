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

float to_float_with_precision(const float value, const int n = 6);

bool AreMatricesEqual(DirectX::XMMATRIX matrix1, DirectX::XMMATRIX matrix2, float epsilon);


struct VertexMesh
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
};

struct VertexPolyLine
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

struct LLACoordinate
{
	float latitude = 0.0;
	float longitude = 0.0;
};

struct CameraPose {

	EpochTime epochtime;

	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	float north = 0.0f;
	float east = 0.0f;
	float down = 0.0f;

	bool operator==(const CameraPose& other) const;
};

std::wstring ToString(const CameraPose& obj);

struct Vector3D
{
public:
	float x;
	float y;
	float z;

	Vector3D operator*(float factor) const;
	Vector3D operator+(const Vector3D& other) const;
	Vector3D operator-(const Vector3D& other) const;
	bool operator==(const Vector3D& other) const;

	Vector3D(const std::array<float, 3> array);
	Vector3D(float x, float y, float z);
	Vector3D(void);

	Vector3D crossProduct(const Vector3D& other) const;


	friend std::ostream& operator<<(std::ostream& os, const Vector3D& obj);

	float squareLength() const;
	Vector3D& normalize();

};

std::wstring ToString(const Vector3D& obj);

Vector3D operator*(float factor, const Vector3D& other);

struct Vector4D
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	Vector4D(const std::array<float, 4> array);
	Vector4D(float x, float y, float z, float w);
	Vector4D(void);

	bool operator==(const Vector4D& other) const;
	DirectX::XMFLOAT4 ToXMFLOAT4(void) const;
};

void Vector3DtoCArray(float array[], Vector3D floats);

void Vector4DtoCArray(float array[], Vector4D floats);

// For soft edges
struct CornerIndices
{
	size_t corner[3];
};

typedef std::vector<CornerIndices> IndicesVec;
typedef std::shared_ptr<IndicesVec> IndicesVecPtr;


struct NormalsInSamePositions
{
	Vector3D meanNormal;
	std::vector<Vector3D> normals;

	void sumNormals();
};

typedef std::map<size_t, NormalsInSamePositions> Map_Ind_Normals;
typedef std::shared_ptr<Map_Ind_Normals> Map_Ind_NormalsPtr;
struct StlVertex
{
	Vector3D pos;
	Vector3D normal;
};

// taken from http://stackoverflow.com/questions/6899392/generic-hash-function-for-all-stl-containers
template <class T>
void hash_combine(std::size_t& seed, const T& v);


struct HTindex_Soft
{
	std::array<float, 3> positions;

	struct Hash
	{
		std::size_t operator()(HTindex_Soft const& key) const;
	};
	
	// equivalence determination.
	bool operator==(HTindex_Soft const& key) const;
};

typedef std::unordered_map<HTindex_Soft, size_t, HTindex_Soft::Hash> HashTable_Soft;

struct stlFacet
{
	float normal[3];
	float position[3][3];
};


struct IRenderableState
{
	unsigned			id = 0;
	std::wstring		name = L"";
	bool				m_isSeen = false;
	Vector3D			rotation = { 0,0,0 };
	Vector3D			translation = { 0,0,0 };
	Vector3D			scale = { 0,0,0 };
	Vector4D			color = { 0,0,0,0 };
};

struct TrajectoryState
{
	unsigned			id			= 0;
	std::wstring		name		= L"";
	bool				m_isSeen	= false;
	Vector3D			rotation	= {0,0,0};
	Vector3D			translation = { 0,0,0 };
};

struct MeshState
{
	unsigned			id			= 0;
	std::wstring		name		= L"";
	bool				m_isSeen	= false;
	Vector4D			color		= { 0,0,0,0 };
};

struct MeshGroupState
{
	Vector3D						rotation = { 0,0,0 };
	Vector3D						scale = { 0,0,0 };
	Vector3D						translation = { 0,0,0 };
	std::vector<MeshState>			Meshes;

};

struct SunPositionState
{
	double azimuth = 0;
	double elevation = 0;
};
struct FlythroughState
{
	bool							IsTrajectoryInitialized		= false;
	float							speed						= 1.0f;
	unsigned						currentFrame				= 0;
	unsigned						numberOfFrame				= 0;
	EpochTime						currentEpochTime			= { 0,0 };
	EpochTime						startEpochTime				= { 0,0 };
	Vector3D						currentCameraPosition		= { 0,0,0 };
	Vector3D						currentCameraRotation		= { 0,0,0 };
	SunPositionState				currentSunPosition;
	LLACoordinate					origo;
	std::vector<TrajectoryState>	trajectoryPolyLine;
};

struct Explore3DState
{
	float				speed;
	float				rotationSpeed;
	EpochTime			currentEpochTime			= { 0,0 };
	Vector3D			currentCameraPosition		= { 0,0,0 };
	Vector3D			currentCameraRotation		= { 0,0,0 };
	LLACoordinate		origo;
	SunPositionState	currentSunPosition;
};

struct GeneralModelState
{	//Camera state
	float fieldOfView; float screenNear; float screenDepth;
	//General configuration
	bool isGridSeen; bool isShadingOn;
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
