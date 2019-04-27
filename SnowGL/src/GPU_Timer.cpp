#include "PCH.h"
#include "GPU_Timer.h"

namespace SnowGL
{
	GPU_Timer::GPU_Timer()
	{
		glCreateQueries(GL_TIME_ELAPSED, 1, &m_query);
	}

	GPU_Timer::~GPU_Timer()
	{
		if (m_started)
		{
			LOG(LOG_WARNING) << "GPU_Timer destoryed before timer stopped";
			glEndQuery(GL_TIME_ELAPSED);
		}
		glDeleteQueries(1, &m_query);
	}

	void GPU_Timer::start()
	{
		if (!m_started)
		{
			m_started = true;
			glBeginQuery(GL_TIME_ELAPSED, m_query);
			return;
		}
		else
		{
			LOG(LOG_ERROR) << "Unable to start timer, timer already started";
		}
	}

	void GPU_Timer::end()
	{
		if (!m_started)
		{
			LOG(LOG_ERROR) << "Unable to stop timer, timer is not started";
			return;
		}
		else
		{
			glEndQuery(GL_TIME_ELAPSED);
			glGetQueryObjectuiv(m_query, GL_QUERY_RESULT, &m_duration);
			m_started = false;
		}

	}

	unsigned int GPU_Timer::getDuration()
	{
		if (!m_started)
		{
			return (unsigned int)m_duration;
		}
		else
		{
			LOG(LOG_ERROR) << "Unable to get duration, timer still running";
		}

		return 0;
	}
}