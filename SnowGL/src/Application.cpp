// cstdlib

// external libs

// program
#include "PCH.h"
#include "InitManager.h"
#include "Window.h"
#include "ShaderProgram.h"

#undef main

static Uint32 next_time;

using namespace SnowGL;

int main()
{
	InitManager::initSDL();

	Window window("SnowGL");

	InitManager::initOpenGL();

	ShaderProgram program("resources/shaders/vert.glsl", "resources/shaders/frag.glsl");

	Mesh mesh;
	IOUtilities::loadMesh(mesh, "resources/models/mk2.obj");

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
	

	std::cout << "Hello World!" << std::endl;


	return 0;
}