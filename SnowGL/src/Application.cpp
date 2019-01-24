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
	srand(1);

	ApplicationState &state = ApplicationState::getInstance();

	InitManager::initSDL();

	Window window("SnowGL");

	InitManager::initOpenGL();

	Camera camera(1280, 720);
	camera.transform.translate(glm::vec3(0, 2, 12));
	camera.updateCameraUniform();

	// create shader
	ShaderProgram shader("resources/shaders/Basic.vert", "resources/shaders/Basic.frag");
	ShaderProgram outlineShader("resources/shaders/Basic.vert", "resources/shaders/BlockColour.frag");

	Texture texture("MK2_Diffuse", "resources/textures/base.png");

	shader.setUniform1i("u_diffuseTexture", 0);
	texture.bind(0);

	// create mesh
	Mesh mesh;
	Mesh debugPlane;

	IOUtilities::loadMesh(mesh, "resources/models/Debug_Plane.obj");
	IOUtilities::loadMesh(debugPlane, "resources/models/Debug_Plane.obj");

	GPU_Mesh openGLMesh;
	GPU_Mesh gpu_DebugPlane;

	openGLMesh.setMesh(mesh);
	gpu_DebugPlane.setMesh(debugPlane);

	// create transform
	Transform transform;
	transform.translate(glm::vec3(0, 0, 0));	
	Transform transformUpscale = transform;
	transformUpscale.scale(glm::vec3(1.05f));	
	Transform transformDownscale = transform;
	transformDownscale.scale(glm::vec3(0.95f));

	Transform zeroTransform;

	Renderer renderer;
	GUI gui(window.getWindowPtr());

	ParticleSettings settings;
	settings.lifetimeMin = 3.0f;
	settings.lifetimeMax = 6.0f;
	settings.particlesPerSecond = 1000;

	SnowfallSystem snow(settings);
	snow.initialise();

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	bool quit = false;
	SceneMode mode = MODE_VIEW;

	float cameraMoveSpeed;

	SDL_ShowCursor(SDL_DISABLE);
	

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
		if (timepassed > lastTime) 
		{
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

			state.cameraYaw += fixedOffset.x;
			state.cameraPitch -= fixedOffset.y;

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
		}

		const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
		if (keyboardState[SDL_SCANCODE_LSHIFT])
		{
			cameraMoveSpeed = 10;
		}
		else
		{
			cameraMoveSpeed = 5;

		}
		if (keyboardState[SDL_SCANCODE_W]) 
		{
			camera.transform.translate(camera.getFront() * cameraMoveSpeed * state.deltaTime);
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			camera.transform.translate(-camera.getFront() * cameraMoveSpeed * state.deltaTime);
		}
		if (keyboardState[SDL_SCANCODE_A])
		{
			glm::vec3 leftVector = glm::cross(glm::vec3(0, 1, 0), camera.getFront());
			camera.transform.translate(leftVector * cameraMoveSpeed * state.deltaTime);
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			glm::vec3 rightVector = glm::cross(camera.getFront(), glm::vec3(0, 1, 0));
			camera.transform.translate(rightVector * cameraMoveSpeed * state.deltaTime);
		}		
		if (keyboardState[SDL_SCANCODE_E])
		{
			camera.transform.translate(glm::vec3(0, 1, 0) * cameraMoveSpeed * state.deltaTime);
		}		
		if (keyboardState[SDL_SCANCODE_Q])
		{
			camera.transform.translate(glm::vec3(0, -1, 0) * cameraMoveSpeed * state.deltaTime);
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

		
		glStencilMask(1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		snow.updateParticles(state.deltaTime);
	
		camera.updateCameraUniform();

		// first pass
		renderer.setStencilBufferActive(true);
		renderer.render(openGLMesh, shader, transform);

		if (state.getSceneMode() == MODE_EDIT)
		{
			outlineShader.setUniform4f("diffuseColour", 0.5f, 0.5f, 0.5f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			renderer.render(gpu_DebugPlane, outlineShader, zeroTransform);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			// second pass (scaled)
			renderer.setStencilBufferActive(false);
			renderer.setDepthTest(false);

			outlineShader.setUniform4f("diffuseColour", 0.0f, 0.8f, 0.0f, 1.0f);
			renderer.render(openGLMesh, outlineShader, transformUpscale);
			renderer.render(openGLMesh, outlineShader, transformDownscale);

			renderer.setStencilBufferActive(true);
			renderer.setDepthTest(true);
		}

		gui.onRender();

		window.swapBuffer();
	}

	std::cout << "Hello World!" << std::endl;


	return 0;
}