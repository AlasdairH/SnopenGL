// cstdlib

// external libs

// program
#include "PCH.h"
#include "ApplicationState.h"
#include "InitManager.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "GPU_Mesh.h"
#include "Renderer.h"
#include "Camera.h"
#include "Timer.h"
#include "SnowfallSystem.h"
#include "GUI.h"

#undef main

using namespace SnowGL;

int main()
{
	ApplicationState &state = ApplicationState::getInstance();

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
	IOUtilities::loadMesh(mesh, "resources/models/barrel.obj");
	GPU_Mesh openGLMesh;
	openGLMesh.setMesh(mesh);

	// create transform
	Transform transform;
	transform.translate(glm::vec3(0, 0, -8));

	Renderer renderer;
	GUI gui(window.getWindowPtr());

	SnowfallSystem snow;
	snow.initialise();
	snow.updateParticles(0);

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	bool quit = false;
	SceneMode mode = MODE_VIEW;

	float cameraMoveSpeed = 5.0f;

	// fps counter variables
	int frames = 0;
	float fps = 0.0f;
	Timer runtime;
	float startTime = runtime.getDuration().count();
	float lastTime = 0;
	float deltaTime = 0;

	while (state.isRunning)
	{
		// calculate FPS
		float timepassed = runtime.getDuration().count();
		frames++;
		if (timepassed - startTime > 0.25 && frames > 10)
		{
			state.framesPerSecond = (double)frames / (timepassed - startTime);
			startTime = timepassed;
			frames = 0;
		}
		// calculate deltaTime
		if (timepassed > lastTime) {
			state.deltaTime = ((float)(timepassed - lastTime));
			lastTime = timepassed;
		}



		// START INPUT
		SDL_Event incomingEvent;
		
		// get mouse position
		int mousePosX, mousePosY;
		SDL_GetMouseState(&mousePosX, &mousePosY);

		while (SDL_PollEvent(&incomingEvent))
		{
			if (incomingEvent.type == SDL_QUIT)
			{
				state.isRunning = false;
			}
			if (incomingEvent.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (incomingEvent.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					state.isRunning = false;
					break; 
				case SDLK_SPACE:
					state.isMenuBarHidden = !state.isMenuBarHidden;
				break; 
				case SDLK_a:
					camera.transform.translate(glm::vec3(-cameraMoveSpeed, 0, 0) * state.deltaTime);
					break;
				case SDLK_d:
					camera.transform.translate(glm::vec3(cameraMoveSpeed, 0, 0) * state.deltaTime);
					break;
				case SDLK_w:
					camera.transform.translate(glm::vec3(0, cameraMoveSpeed, 0) * state.deltaTime);
					break;
				case SDLK_s:
					camera.transform.translate(glm::vec3(0, -cameraMoveSpeed, 0) * state.deltaTime);
					break;

				default:
					break;
				}
			}
		}

		gui.onUpdate();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		//transform.rotate(1, glm::vec3(0, 1, 0));
		camera.updateCameraUniform();
		renderer.render(openGLMesh, shader, transform);
	
		gui.onRender();

		window.swapBuffer();
	}

	std::cout << "Hello World!" << std::endl;


	return 0;
}