#include "PCH.h"
#include "ApplicationState.h"

namespace SnowGL
{
	void ApplicationState::switchSceneMode()
	{
		if (m_sceneMode == MODE_EDIT)
		{
			m_sceneMode = MODE_VIEW;

			renderWireframe = false;
		}
		else if (m_sceneMode == MODE_VIEW)
		{
			m_sceneMode = MODE_EDIT;

			renderWireframe = true;
		}
	}
}