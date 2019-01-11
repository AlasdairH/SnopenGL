// cstdlib

// external libs

// program
#include "PCH.h"
#include "InitManager.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "GPU_Mesh.h"
#include "Renderer.h"
#include "Camera.h"
#include "SnowfallSystem.h"

#undef main

static Uint32 next_time;

using namespace SnowGL;

int main()
{
	InitManager::initSDL();

	Window window("SnowGL");

	InitManager::initOpenGL();

	Camera camera(1280, 720);
	camera.transform.translate(glm::vec3(0, 2, 0));
	camera.updateCameraUniform();

	// create shader
	ShaderProgram shader("resources/shaders/vert.vert", "resources/shaders/frag.frag");

	// create mesh
	Mesh mesh;
	IOUtilities::loadMesh(mesh, "resources/models/deer.obj");
	GPU_Mesh openGLMesh;
	openGLMesh.setMesh(mesh);

	// create transform
	Transform transform;
	transform.translate(glm::vec3(0, 0, -8));

	Renderer renderer;


	SnowfallSystem snow;
	snow.initialise();
	snow.updateParticles(0);

	int m_currVB, m_currTFB = 1;

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
			if (incomingEvent.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (incomingEvent.key.keysym.sym)
				{
				case SDLK_RETURN:

					break;

				default:
					break;
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		transform.rotate(1, glm::vec3(0, 1, 0));
		renderer.render(openGLMesh, shader, transform);
		
		window.swapBuffer();
	}
	

	std::cout << "Hello World!" << std::endl;


	return 0;
}