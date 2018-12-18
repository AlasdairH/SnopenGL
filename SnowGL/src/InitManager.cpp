#include "PCH.h"
#include "InitManager.h"

namespace SnowGL
{
	void InitManager::initSDL()
	{
		CONSOLE_MESSAGE("Initialising SDL");
		// attempt to initialise opengl
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
		{
			// something went wrong, exit program
			CONSOLE_ERROR("Unable to Initialise SDL");
		}

		// set OpenGL 4.3
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		CONSOLE_MESSAGE("SDL Initialised");
	}

	void InitManager::initOpenGL()
	{
		CONSOLE_MESSAGE("Initialising OpenGL");

		glewExperimental = GL_TRUE;

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			CONSOLE_ERROR("GLEW failed to initialise ");
			return;
		}

		CONSOLE_MESSAGE("Using GLEW " << glewGetString(GLEW_VERSION));

		CONSOLE_MESSAGE("OpenGL Vendor: " << glGetString(GL_VENDOR));
		CONSOLE_MESSAGE("OpenGL Renderer: " << glGetString(GL_RENDERER));
		CONSOLE_MESSAGE("OpenGL Version: " << glGetString(GL_VERSION));
		CONSOLE_MESSAGE("OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION));

		CONSOLE_MESSAGE("OpenGL Initialised");
	}
}

