#ifndef TIMER_H
#define TIMER_H
#include <chrono>

///////////////////////////////////////////////////////////////////////////////
// Timer.h
// =======
// A class that provides a high-resolution timer for measuring elapsed time.
//
// It can be used to measure elapsed time between two points in the code and to control frame rate in animations and simulations.
//
// To use the Timer class, an instance of the class must be created, and the Start() function called to start the timer.
// The elapsed time can be measured by calling the GetMilisecondsElapsed() function.
//
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////

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
