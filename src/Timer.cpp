#include "Timer.hpp"

Timer::Timer()
{
	this->start();
}

void Timer::start()
{
	m_start = std::chrono::high_resolution_clock::now();
}

void Timer::restart()
{
	this->start();
}

float Timer::getElapsedTime()
{
	m_stop = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(m_stop - m_start).count() * 0.000001;
}