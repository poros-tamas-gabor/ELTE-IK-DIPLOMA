#ifndef FILE_SELECT_EVENT_H
#define FILE_SELECT_EVENT_H

#include <string>
class FileSelectEvent
{
public:
	enum Type
	{
		Invalid,
		TerrainFile,
		CameraTrajectoryFile,
		CameraPropertiesFile
	};
private:
	FileSelectEvent::Type	_type;
	std::wstring		_filepath;

public:
	FileSelectEvent();
	FileSelectEvent(FileSelectEvent::Type type, std::wstring filepath);

	bool IsTerrainFile() const;
	bool IsCameraTrajectoryFile() const;
	bool IsCameraPropertiesFile() const;
	bool IsValid() const;
	std::wstring GetFilePath() const;
};
#endif