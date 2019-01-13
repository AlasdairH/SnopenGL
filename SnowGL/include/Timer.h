#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

class Timer
{
public:
	Timer() { start = std::chrono::high_resolution_clock::now(); }
	~Timer() { duration = std::chrono::high_resolution_clock::now() - start; std::cout << "INFO: Timer Took " << duration.count() * 1000.0f << "ms" << std::endl; }

	std::chrono::duration<float> getDuration() { return std::chrono::high_resolution_clock::now() - start; }

protected:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::duration<float> duration;
};