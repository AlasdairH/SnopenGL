// cstdlib

// external libs

// program
#include "PCH.h"

#undef main

static Uint32 next_time;

int main()
{
	SnowGL::InitManager::initSDL();

	SnowGL::Window window("SnowGL");

	SnowGL::InitManager::initOpenGL();

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	bool quit = false;

	while (!quit)
	{
		// START INPUT
		SDL_Event incomingEvent;
		while (SDL_PollEvent(&incomingEvent))
		{
			if (incomingEvent.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		window.swapBuffer();
	}


	return 0;
}