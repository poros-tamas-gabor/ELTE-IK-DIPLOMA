#ifndef FILE_SELECT_EVENT_H
#define FILE_SELECT_EVENT_H

#include <string>
#include "EventTemplate.h"
namespace FileSelectEvent
{
	enum Type
	{
		Invalid,
		TerrainFile,
		CameraTrajectoryFile,
		CameraPropertiesFile
	};
	typedef ::Event<Type, std::wstring> Event;
}

#endif