#ifndef CONTROLLER_EVENT_H
#define CONTROLLER_EVENT_H

#include <string>
#include "IEvent.h"
#include <string>

namespace ControllerEvent
{
    class Event : public IEvent
    {
    private:
        ControllerEvent::Type   m_type;

    public:
        virtual ControllerEvent::Type GetEventType() { return m_type; }
        virtual void SetEventType(ControllerEvent::Type type) { m_type = type; }
    };

    class NewFrameEvent : public ControllerEvent::IEvent {
    private:
        float m_elapsedMiliseconds;
    public:
        virtual void SetEventType(ControllerEvent::Type) {}
        ControllerEvent::Type GetEventType() override { return NewFrame; }
        void SetElapsedMiliseconds(float dt) { m_elapsedMiliseconds = dt; }
        float GetElapsedMiliseconds(void) { return m_elapsedMiliseconds; }

    };

    class FileSelectEvent : public ControllerEvent::IEvent {
    protected:
        std::wstring            m_filepath;
        ControllerEvent::Type   m_type;
    public:

        virtual void SetEventType(ControllerEvent::Type type) { m_type = type; }
        ControllerEvent::Type GetEventType() { return m_type; };
        void SetFilePath(const wchar_t * filepath) { m_filepath = filepath; }
        const wchar_t* GetFilePath(void) { return m_filepath.c_str(); }

    };

    class TerrainFileSelectEvent : public FileSelectEvent
    {
        ControllerEvent::Type GetEventType() override { return ClickButton_SetTerrainPath; }
    };

    class EnterFloatEvent : public IEvent
    {
    protected:
        float                   m_data;
        ControllerEvent::Type   m_type;
    public:
        virtual void SetEventType(ControllerEvent::Type type) { m_type = type; }
        ControllerEvent::Type GetEventType() { return m_type; };
        void SetFloatData(float data) { m_data = data; }
        float GetFloatData(void) { return m_data; }

    };

}

#endif