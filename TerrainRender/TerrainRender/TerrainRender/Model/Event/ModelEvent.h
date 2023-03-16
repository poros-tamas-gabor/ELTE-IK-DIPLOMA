#ifndef MODEL_EVENT_H
#define MODEL_EVENT_H

#include <string>
#include "IEvent.h"
namespace ModelEvent
{

    class Event : public IEvent
    {
    protected:
        Type m_type;
    public:
        virtual void SetEventType(Type type) { m_type = type; }
        virtual Type GetEventType() override { return m_type; }
    };

    class SetFloatData : public IEvent {
    protected:
        float m_data;
        Type m_type;
    public:
        virtual void SetEventType(Type type) { m_type = type; }
        virtual Type GetEventType() override { return m_type; }
        void SetData(float data) { m_data = data; }
        float GetData(void) { return m_data; }

    };
    class SetCameraMoveSpeedEvent : public SetFloatData {
    public: 
        virtual Type GetEventType() override { return Type::SetCameraMoveSpeed; }
    private:
        virtual void SetEventType(Type type) { m_type = type; }
    };

    class SetCameraRotationSpeedEvent : public SetFloatData {
    public:
        virtual Type GetEventType() override { return Type::SetCameraRotationSpeed; }
    private:
        virtual void SetEventType(Type type) { m_type = type; }
    };
}

#endif