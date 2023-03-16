#ifndef CONTROLLER_EVENT_H
#define CONTROLLER_EVENT_H

#include <string>
#include "IEvent.h"
#include <string>

namespace ControllerEvent
{
    class NewFrameEvent : public ControllerEvent::IEvent {
    private:
        float m_elapsedMiliseconds;
    public:

        ControllerEvent::Type GetEventType() override { return NewFrame; }
        void SetElapsedMiliseconds(float dt) { m_elapsedMiliseconds = dt; }
        float GetElapsedMiliseconds(void) { return m_elapsedMiliseconds; }

    };

    class FileSelectEvent : public ControllerEvent::IEvent {
    protected:
        std::wstring m_filepath;
    public:

        ControllerEvent::Type GetEventType() = 0;
        void SetFilePath(const wchar_t * filepath) { m_filepath = filepath; }
        const wchar_t* GetFilePath(void) { return m_filepath.c_str(); }

    };

    class TerrainFileSelectEvent : public FileSelectEvent
    {
        ControllerEvent::Type GetEventType() override { return ClickButton_SetTerrainPath; }
    };
}

#endif