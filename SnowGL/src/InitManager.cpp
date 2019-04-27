#include "PCH.h"
#include "InitManager.h"

namespace SnowGL
{
	void InitManager::initSDL()
	{
		LOG(LOG_DEBUG) << "Initialising SDL";
		// attempt to initialise openGL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
		{
			// something went wrong, exit program
			LOG(LOG_ERROR) << "Unable to Initialise SDL";
		}

		// set OpenGL 4.3
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// set attributes
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

		LOG(LOG_DEBUG) << "SDL Initialised";
	}

	void InitManager::initOpenGL()
	{
		LOG(LOG_DEBUG) << "Initialising OpenGL";

		glewExperimental = GL_TRUE;

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			LOG(LOG_ERROR) << "GLEW failed to initialise ";
			return;
		}

		LOG(LOG_DEBUG) << "Using GLEW " << glewGetString(GLEW_VERSION);

		LOG(LOG_DEBUG) << "OpenGL Vendor: " << glGetString(GL_VENDOR);
		LOG(LOG_DEBUG) << "OpenGL Renderer: " << glGetString(GL_RENDERER);
		LOG(LOG_DEBUG) << "OpenGL Version: " << glGetString(GL_VERSION);
		LOG(LOG_DEBUG) << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);

		int maxTexBufferSize;
		glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxTexBufferSize);
		LOG(LOG_DEBUG) << "Max Texture Buffer Size: " << maxTexBufferSize;
		int maxSSBOSize;
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSSBOSize);
		LOG(LOG_DEBUG) << "Max SSBO Size: " << maxSSBOSize;

		LOG(LOG_DEBUG) << "OpenGL Initialised";
	}
}

