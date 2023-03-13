#include "FileSelectEvent.h"

FileSelectEvent::FileSelectEvent() : _type(Type::Invalid), _filepath(L"") {};
FileSelectEvent::FileSelectEvent(FileSelectEvent::Type type, std::wstring filepath) : _type(type), _filepath(filepath) {};

bool FileSelectEvent::IsTerrainFile() const
{
	return this->_type == FileSelectEvent::Type::TerrainFile;
}

bool FileSelectEvent::IsCameraTrajectoryFile() const
{
	return this->_type == FileSelectEvent::Type::CameraTrajectoryFile;

}
bool FileSelectEvent::IsCameraPropertiesFile() const
{
	return this->_type == FileSelectEvent::Type::CameraPropertiesFile;
}
bool FileSelectEvent::IsValid() const
{
	return this->_type != FileSelectEvent::Type::Invalid;

}
std::wstring FileSelectEvent::GetFilePath() const
{
	return this->_filepath;
}
