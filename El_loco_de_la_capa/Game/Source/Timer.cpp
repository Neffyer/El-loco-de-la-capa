#include "Timer.h"
#include "SDL\include\SDL_timer.h"
#include "Log.h"

// L13: TODO 1: Fill Start(), Read(), ReadSec() methods
// they are simple, one line each!

Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	startTime = SDL_GetTicks();
}

uint32 Timer::ReadSec() const
{
	return (SDL_GetTicks() - startTime) / 1000;
}

float Timer::ReadMSec() const
{
	return (float)(SDL_GetTicks() - startTime);
}