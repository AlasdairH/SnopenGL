#include "PCH.h"
#include "ApplicationState.h"

namespace SnowGL
{
	void ApplicationState::switchSceneMode()
	{
		if (m_sceneMode == MODE_EDIT)
		{
			m_sceneMode = MODE_VIEW;

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (m_sceneMode == MODE_VIEW)
		{
			m_sceneMode = MODE_EDIT;

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
}