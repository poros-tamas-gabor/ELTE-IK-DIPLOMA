#ifndef FILE_SELECT_EVENT_H
#define FILE_SELECT_EVENT_H

#include <string>
#include "../EventTemplate.h"

enum FileSelectEvent_Type
{
	Invalid,
	TerrainFile,
	CameraTrajectoryFile,
	CameraPropertiesFile
};

typedef Event<FileSelectEvent_Type, std::wstring> FileSelectEvent;


#endif