#ifndef MODEL_STRUCTS_H
#define MODEL_STRUCTS_H

#include "EpochTime.h"

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
	Vector3D normal;
	Vector3D position[3];
};

enum STLLineType { BEGIN, FACET, LOOP, VERTEX, ENDLOOP, ENDFACET, END };
#endif // !MODEL_VERTEX_H
