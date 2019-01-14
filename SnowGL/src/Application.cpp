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
#include "Texture.h"

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

	Texture texture("test", "resources/textures/base.png");

	// create mesh
	Mesh mesh;
	IOUtilities::loadMesh(mesh, "resources/models/MK2.obj");
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
		state.lastMousePosition = state.mousePosition;
		state.mousePosition = glm::vec2(mousePosX, mousePosY);
		state.mouseOffset = state.mousePosition - state.lastMousePosition;

		// if right mouse button is being held
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) 
		{
			glm::vec2 fixedOffset = state.mouseOffset * 0.05f;

			state.cameraYaw -= fixedOffset.x;
			state.cameraPitch += fixedOffset.y;

			if (state.cameraPitch > 89.0f)
				state.cameraPitch = 89.0f;
			if (state.cameraPitch < -89.0f)
				state.cameraPitch = -89.0f;

			glm::vec3 front;
			front.x = cos(glm::radians(state.cameraPitch)) * cos(glm::radians(state.cameraYaw));
			front.y = sin(glm::radians(state.cameraPitch));
			front.z = cos(glm::radians(state.cameraPitch)) * sin(glm::radians(state.cameraYaw));
			front = glm::normalize(front);
			camera.setFront(front);

			CONSOLE_MESSAGE(front.x << ", " << front.y << ", " << front.z);
			CONSOLE_MESSAGE(state.cameraPitch << ", " << state.cameraYaw);
		}

		const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
		if (keyboardState[SDL_SCANCODE_W]) 
		{
			camera.transform.translate(glm::vec3(0, 0, -cameraMoveSpeed) * state.deltaTime);
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			camera.transform.translate(glm::vec3(0, 0, cameraMoveSpeed) * state.deltaTime);
		}
		if (keyboardState[SDL_SCANCODE_A])
		{
			camera.transform.translate(glm::vec3(-cameraMoveSpeed, 0, 0) * state.deltaTime);
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			camera.transform.translate(glm::vec3(cameraMoveSpeed, 0, 0) * state.deltaTime);
		}
		
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
	
		texture.bind();
		gui.onRender();

		window.swapBuffer();
	}

	std::cout << "Hello World!" << std::endl;


	return 0;
}