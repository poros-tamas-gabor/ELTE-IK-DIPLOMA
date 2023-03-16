#ifndef EVENT_TEMPLATE_H
#define EVENT_TEMPLATE_H
template <typename T, typename D> class Event {
private:
	T _type;
	D _data;

public:
	bool IsType(T type) const { return type == _type; };
	D GetData(void) const { return _data; };

	Event() : _type(0) {}
	Event(T type, D data) : _type(type), _data(data) {}
};
#endif