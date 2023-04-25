#include "ModelStructs.h"

float to_float_with_precision(const float value, const int n)
{
	return std::roundf(value * std::pow(10, n)) / std::pow(10, n);
}
void Vector3DtoCArray(float array[], Vector3D floats)
{
	array[0] = floats.x;
	array[1] = floats.y;
	array[2] = floats.z;
}

void Vector4DtoCArray(float array[], Vector4D floats)
{
	array[0] = floats.x;
	array[1] = floats.y;
	array[2] = floats.z;
	array[3] = floats.w;
}
template<class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

std::size_t HTindex_Soft::Hash::operator()(HTindex_Soft const& key) const
{
	size_t h = std::hash<float>()(key.positions[0]);
	hash_combine(h, key.positions[1]);
	hash_combine(h, key.positions[2]);
	return h;
}


// equivalence determination.
bool HTindex_Soft::operator==(HTindex_Soft const& key) const
{
	return positions == key.positions;
}


bool CameraPose::operator==(const CameraPose& o) const
{
	return down == o.down && east == o.east && epochtime == o.epochtime && north == o.north && pitch == o.pitch && roll == o.roll && yaw == o.yaw;
}

std::wstring ToString(const CameraPose& obj)
{
	std::wostringstream oss;
	oss << L"{ " << obj.north << L", " << obj.east << L", " << obj.east << L" } / { " << obj.pitch << L", " << obj.yaw << L", " << obj.roll << L" }" ;
	return oss.str();
}

Vector3D Vector3D::operator*(float factor) const
{
	return { this->x * factor, this->y * factor, this->z * factor };
}

Vector3D Vector3D::operator+(const Vector3D& other) const
{
	return { this->x + other.x, this->y + other.y, this->z + other.z };
}
Vector3D Vector3D::operator-(const Vector3D& other) const
{
	return { this->x - other.x, this->y - other.y, this->z - other.z };
}

bool Vector3D::operator==(const Vector3D& other) const
{
	return { this->x == other.x && this->y == other.y && this->z == other.z };
}

Vector3D::Vector3D(const std::array<float, 3> array) : x(array.at(0)), y(array.at(1)), z(array.at(2)) {}
Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
Vector3D::Vector3D(void) : x(0), y(0), z(0) {}

Vector3D Vector3D::crossProduct(const Vector3D& other) const {
	float cx = y * other.z - z * other.y;
	float cy = z * other.x - x * other.z;
	float cz = x * other.y - y * other.x;
	return Vector3D(cx, cy, cz);
}

 std::ostream& operator<<(std::ostream& os, const Vector3D& obj) {
	return os << "{ " << obj.x << ", " << obj.y << ", " << obj.z << " }";
}


float Vector3D::squareLength() const {
	return x * x + y * y + z * z;
}

Vector3D& Vector3D::normalize()
{
	float sqaureLength = this->squareLength();
	if (sqaureLength > 0)
	{
		float length = std::sqrt(sqaureLength);
		x /= length;
		y /= length;
		z /= length;
	}
	return *this;
}


std::wstring ToString(const Vector3D& obj)
{
	std::wostringstream oss;
	oss << L"{ " << obj.x << L", " << obj.y << L", " << obj.z << L" }";
	return oss.str();
}

DirectX::XMFLOAT4 Vector4D::ToXMFLOAT4(void) const
{
	return { x,y,z,w };
}
bool Vector4D::operator==(const Vector4D& other) const
{
	return { this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w };
}

Vector4D::Vector4D(const std::array<float, 4> array) : x(array.at(0)), y(array.at(1)), z(array.at(2)), w(array.at(3)) {}
Vector4D::Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
Vector4D::Vector4D(void) : x(0), y(0), z(0), w(0) {}

Vector3D operator*(float factor, const Vector3D& other)
{
	return other * factor;
}

void NormalsInSamePositions::sumNormals()
{
	Vector3D sol = { 0, 0, 0 };
	for (const Vector3D& v : normals)
	{
		sol = sol + v;
	}
		
	sol.normalize();
	meanNormal = sol;
}
