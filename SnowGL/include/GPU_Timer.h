#pragma once

// cstdlib


// external libs

// program
#include "PCH.h"

// TODO: Doxygen

namespace SnowGL
{
	class GPU_Timer
	{
	public:
		GPU_Timer();
		~GPU_Timer();

		void start();
		void end();

		inline bool isRunning() { return m_started; }

		unsigned int getDuration();

	protected:
		bool	m_started = false;
		GLuint	m_query;
		GLuint	m_duration; // nanoseconds
	};
}