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
#include "ParticleSystem.h"
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

	Camera depthCamera(1280, 720);
	depthCamera.transform.translate(glm::vec3(0, 5, 0));
	//depthCamera.setFOV(glm::radians(20.0f));
	depthCamera.setPitch(-89.9f);
	depthCamera.setProjectionMode(PROJECTION_ORTHOGRAPHIC);

	// create a camera data uniform buffer
	std::shared_ptr<VertexBuffer> cameraDataUniformBuffer = std::make_shared<VertexBuffer>(BUFFER_UNIFORM);
	// link the uniform buffer to the binding point
	cameraDataUniformBuffer->bindBase(GL_UNIFORM_BUFFER, SHADER_BINDPOINT_CAMERA_VP);
	// load the data to the uniform buffer
	cameraDataUniformBuffer->loadData(&camera.getCameraUniformData(), 0, sizeof(u_CameraData));

	// create shader
	//ShaderProgram outlineShader("resources/shaders/Basic.vert", "resources/shaders/BlockColour.frag");

	Transform zeroTransform;

	Renderable groundPlane;
	IOUtilities::loadRenderable(groundPlane, "resources/objects/Plane.rnd");

	Renderable cube;
	IOUtilities::loadRenderable(cube, "resources/objects/Grenade.rnd");
	cube.transform.translate(glm::vec3(0, 0, 0));

	Renderer renderer;

	GUI gui(window.getWindowPtr());

	ParticleSettings settings;
	settings.lifetimeMin = 10.0f;
	settings.lifetimeMax = 10.0f;
	settings.colourStart = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	settings.colourEnd = glm::vec4(0.79f, 0.90f, 0.88f, 1.0f);
	settings.particlesPerSecond = 1;
	settings.globalWind = glm::vec3(0.0f);

	//ParticleSystem snow(settings);
	//snow.initialise();
	//gui.setSelectedParticleSystem(std::make_shared<ParticleSystem>(snow));

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
			state.framesPerSecond = (float)frames / (timepassed - startTime);
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
		if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) && Camera::activeCamera->getProjectionMode() == PROJECTION_PERSPECTIVE) 
		{
			glm::vec2 fixedOffset = state.mouseOffset * 0.05f;

			Camera *cameraPtr = Camera::activeCamera;

			cameraPtr->setYaw(cameraPtr->getYaw() + fixedOffset.x);
			cameraPtr->setPitch(cameraPtr->getPitch() - fixedOffset.y);

			if (cameraPtr->getPitch() > 90.0f)
				cameraPtr->setPitch(90.0);
			if (cameraPtr->getPitch() < -90.0f)
				cameraPtr->setPitch(-90.0f);
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
			Camera::activeCamera->transform.translate(camera.getFront() * cameraMoveSpeed * state.deltaTime);
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			Camera::activeCamera->transform.translate(-camera.getFront() * cameraMoveSpeed * state.deltaTime);
		}
		if (keyboardState[SDL_SCANCODE_A])
		{
			glm::vec3 leftVector = glm::cross(glm::vec3(0, 1, 0), camera.getFront());
			Camera::activeCamera->transform.translate(leftVector * cameraMoveSpeed * state.deltaTime);
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			glm::vec3 rightVector = glm::cross(camera.getFront(), glm::vec3(0, 1, 0));
			Camera::activeCamera->transform.translate(rightVector * cameraMoveSpeed * state.deltaTime);
		}		
		if (keyboardState[SDL_SCANCODE_E])
		{
			Camera::activeCamera->transform.translate(glm::vec3(0, 1, 0) * cameraMoveSpeed * state.deltaTime);
		}		
		if (keyboardState[SDL_SCANCODE_Q])
		{
			Camera::activeCamera->transform.translate(glm::vec3(0, -1, 0) * cameraMoveSpeed * state.deltaTime);
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
					state.isUIHidden = !state.isUIHidden;
					break;
				default:
					break;
				}
			}
		}
		
		glStencilMask(1);
		renderer.setStencilBufferActive(true);

		// 1st pass: render to depth FBO
		renderer.bindDepthFrameBuffer();
		{
			glClear(GL_DEPTH_BUFFER_BIT);

			depthCamera.updateCameraUniform();
			cameraDataUniformBuffer->loadData(&depthCamera.getCameraUniformData(), 0, sizeof(u_CameraData));
			renderer.setDepthSpaceMatrix(depthCamera.getCameraUniformData().projectionMatrix * depthCamera.getCameraUniformData().viewMatrix);

			// render all objects
			renderer.renderToDepthBuffer(groundPlane);
			renderer.renderToDepthBuffer(cube);
		}
		renderer.unBindDepthFrameBuffer();

		// switch camera back to main
		Camera::activeCamera->updateCameraUniform();
		cameraDataUniformBuffer->loadData(&Camera::activeCamera->getCameraUniformData(), 0, sizeof(u_CameraData));

		// 2nd pass: rendering to frame buffer
		renderer.bindFrameBuffer();
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// render all objects
			renderer.render(groundPlane);
			renderer.render(cube);
		}
		renderer.unBindFrameBuffer();

		/*
		// if the scene is in edit mode
		if (state.getSceneMode() == MODE_EDIT)
		{
			// render a plane in wire frame mode at the origin
			outlineShader.setUniform4f("diffuseColour", 0.5f, 0.5f, 0.5f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			renderer.render(*editPlane.getGPUMesh(), outlineShader, zeroTransform);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			// second pass (scaled) for outline
			renderer.setStencilBufferActive(false);
			renderer.setDepthTest(false);

			Transform transformUpscale = editPlane.transform;
			transformUpscale.scale(glm::vec3(1.05f));

			outlineShader.setUniform4f("diffuseColour", 0.0f, 0.8f, 0.0f, 1.0f);
			renderer.render(*editPlane.getGPUMesh(), outlineShader, transformUpscale);

			renderer.setStencilBufferActive(true);
			renderer.setDepthTest(true);
		}
		*/

		if (state.getSceneMode() == MODE_VIEW)
		{
			renderer.drawFrameBuffer();
		}
		else
		{
			renderer.drawDepthFrameBuffer();
		}
		

		// GUI
		gui.onUpdate();
		gui.onRender();

		window.swapBuffer();
	}

	std::cout << "Hello World!" << std::endl;


	return 0;
}