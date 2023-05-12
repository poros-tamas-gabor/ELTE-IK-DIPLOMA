#include "ModelStructs.h"
#include "../../nlohmann/json.hpp"
#include "../../TRException.h"
#include "from_json.h"

Vector3D to_Vector3D(const std::vector<float>& stdvec)
{
    THROW_TREXCEPTION_IF_FAILED((stdvec.size() == 3), L"Failed to load Vector3D");
    Vector3D vec = { stdvec.at(0) , stdvec.at(1) ,stdvec.at(2) };
    return vec;
}

Vector4D to_Vector4D(const std::vector<float>& stdvec)
{
    Vector4D vec;
    THROW_TREXCEPTION_IF_FAILED((stdvec.size() == 4), L"Failed to load Vector4D");
    vec.x = stdvec.at(0);
    vec.y = stdvec.at(1);
    vec.z = stdvec.at(2);
    vec.w = stdvec.at(3);
    return vec;
}

void from_json(const nlohmann::json& json, ParameterFile::Camera& data)
{
    data.fieldOfView = json.at("fieldOfView").get<float>();
    data.screenNear = json.at("nearScreen").get<float>();
    data.screenDepth = json.at("farScreen").get<float>();
}

void from_json(const nlohmann::json& json, ParameterFile::Terrain& data)
{
    std::vector<float> tmp = json.at("translation").get<std::vector<float>>();
    data.translation = to_Vector3D(tmp);
    tmp = json.at("rotation").get<std::vector<float>>();
    data.rotation = to_Vector3D(tmp);
    tmp = json.at("scale").get<std::vector<float>>();
    data.scale = to_Vector3D(tmp);

    for (auto it = json.at("color").begin(); it != json.at("color").end(); ++it) {
        std::string color_name = it.key();
        std::vector<float> color_values;
        for (float v : it.value())
        {
            color_values.push_back(v / 255.0f);
        }
        Vector4D vec = to_Vector4D(color_values);
        data.colors.insert(std::pair<std::string, Vector4D>(color_name, vec));
    }
}

void from_json(const nlohmann::json& json, ParameterFile::Trajectory& data)
{
    std::vector<float> tmp = json.at("translation").get<std::vector<float>>();
    data.translation = to_Vector3D(tmp);
    tmp = json.at("rotation").get<std::vector<float>>();
    data.rotation = to_Vector3D(tmp);
}

void from_json(const nlohmann::json& j, ParameterFile& p) 
{
    p.origo = j.at("origo").get<LLACoordinate>();
    p.camera = j.at("camera").get<ParameterFile::Camera>();
    p.terrain = j.at("terrain").get<ParameterFile::Terrain>();
    p.trajectory = j.at("trajectory").get<ParameterFile::Trajectory>();
}

void from_json(const nlohmann::json& json, LLACoordinate& data)
{
    data.latitude = json.at("latitude").get<float>();
    data.longitude = json.at("longitude").get<float>();
}