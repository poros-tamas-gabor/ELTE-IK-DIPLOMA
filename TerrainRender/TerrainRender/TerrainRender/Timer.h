#ifndef TIMER_H
#define TIMER_H
#include <chrono>

class Timer
{
public:
	Timer();
	double GetMilisecondsElapsed();
	void Restart();
	bool Stop();
	bool Start();
private:
	bool isrunning = false;

	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;

};
#endif // !TIMER_H
