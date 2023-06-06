#include "ModelStructs.h"
#include "../../nlohmann/json.hpp"
#include "../../TRException.h"


Vector3D to_Vector3D(const std::vector<float>& stdvec);
Vector4D to_Vector4D(const std::vector<float>& stdvec);
void from_json(const nlohmann::json& json, ParameterFile::Terrain& data);
void from_json(const nlohmann::json& json, ParameterFile::Camera& data);
void from_json(const nlohmann::json& json, ParameterFile::Trajectory& data);
void from_json(const nlohmann::json& json, LLACoordinate& data);
void from_json(const nlohmann::json& j, ParameterFile& p);


