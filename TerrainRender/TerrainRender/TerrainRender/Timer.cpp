#include "Timer.h"

Timer::Timer()
{
	this->start = std::chrono::steady_clock::now();
	this->stop = std::chrono::steady_clock::now();

}
double Timer::GetMilisecondsElapsed()
{
	if (this->isrunning)
	{
		std::chrono::duration<double, std::milli> elapsed = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		std::chrono::duration<double, std::milli> elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
}
void Timer::Restart()
{
	this->isrunning = true;
	this->start = std::chrono::steady_clock::now();
}
bool Timer::Stop()
{
	if (this->isrunning)
	{
		stop = std::chrono::steady_clock::now();
		isrunning = false;
		return true;
	}
	return false;
}
bool Timer::Start()
{
	if (!this->isrunning)
	{
		this->start = std::chrono::steady_clock::now();
		isrunning = true;
		return true;
	}
	return false;
}