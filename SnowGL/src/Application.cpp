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
	CONSOLE_MESSAGE(sizeof(unsigned int));
	CONSOLE_MESSAGE(sizeof(const void*));

	srand(1);

	ApplicationState &state = ApplicationState::getInstance();

	InitManager::initSDL();

	Window window("SnopenGL");


	InitManager::initOpenGL();

	Camera camera((int)state.windowSize.x, (int)state.windowSize.y, PROJECTION_PERSPECTIVE);
	camera.transform.translate(glm::vec3(0, 3, 18));

	Camera depthCamera(1024, 1024, PROJECTION_ORTHOGRAPHIC);
	depthCamera.transform.translate(glm::vec3(0, 5, 0));
	depthCamera.setPitch(-89.9f);

	// create a camera data uniform buffer
	std::shared_ptr<VertexBuffer> cameraDataUniformBuffer = std::make_shared<VertexBuffer>(BUFFER_UNIFORM);
	// link the uniform buffer to the binding point
	cameraDataUniformBuffer->bindBase(GL_UNIFORM_BUFFER, SHADER_BINDPOINT_CAMERA_VP);
	// load the data to the uniform buffer
	cameraDataUniformBuffer->loadData(&camera.getCameraUniformData(), 0, sizeof(GPU_UB_CameraData));

	// create shader
	//ShaderProgram outlineShader("resources/shaders/Basic.vert", "resources/shaders/BlockColour.frag");

	Transform zeroTransform;

	// create a ground plane (grass)
	Renderable groundPlane;
	IOUtilities::loadRenderable(groundPlane, "resources/objects/Plane.rnd");
	Renderable groundPlane_COLLISION;
	IOUtilities::loadRenderable(groundPlane_COLLISION, "resources/objects/Plane_Collision.rnd");

	// create a scene object
	Renderable sceneObject;
	IOUtilities::loadRenderable(sceneObject, "resources/objects/Table.rnd");
	sceneObject.transform.translate(glm::vec3(1.5f, 0, 0));
	sceneObject.transform.rotate(-45, glm::vec3(0, 1, 0));
	sceneObject.m_shader->setUniform1i("u_useSnowTexture", 1);
	sceneObject.m_shader->setUniform1i("u_useSnowOffset", 1);
	Renderable sceneObject_COLLISION;
	IOUtilities::loadRenderable(sceneObject_COLLISION, "resources/objects/Table_Collision.rnd");	
	// create a scene object
	Renderable sceneObject2;
	IOUtilities::loadRenderable(sceneObject2, "resources/objects/Bin.rnd");
	sceneObject2.transform.translate(glm::vec3(-1, 0, 0));
	//sceneObject2.transform.rotate(45, glm::vec3(0, 1, 0));
	sceneObject2.m_shader->setUniform1i("u_useSnow", 1);
	sceneObject2.m_shader->setUniform1i("u_useSnowOffset", 1);
	Renderable sceneObject2_COLLISION;
	IOUtilities::loadRenderable(sceneObject2_COLLISION, "resources/objects/Bin_Collision.rnd");

	// create a texture buffer to contain collidable world space geometry
	VertexBuffer vboGeometry(BUFFER_ARRAY);
	vboGeometry.addTextureBuffer(GL_RGBA32F, 1024 * 1024 * sizeof(glm::vec4));
	VertexArray vaoGeometry;
	VertexBufferLayout layout;
	layout.push<glm::vec4>(1);
	vaoGeometry.addBuffer(vboGeometry, layout);

	int vertexCount = 0;
	vertexCount += groundPlane_COLLISION.getVertexCount();
	vertexCount += sceneObject_COLLISION.getVertexCount();
	vertexCount += sceneObject2_COLLISION.getVertexCount();
	int triangleCount = vertexCount / 3;

	Renderer renderer;

	GUI gui(window.getWindowPtr());

	ParticleSettings settings;
	settings.lifetimeMin = 10.0f;
	settings.lifetimeMax = 10.0f;
	settings.particlesPerSecond = 20000;
	// colour
	settings.colourStart = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	settings.colourEnd = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	settings.collisionDebugColour = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	// physics
	settings.globalWind = glm::vec3(0.0f, 0.0f, 0.0f);
	settings.initialVelocity = glm::vec3(0, -1.0f, 0);
	// debug
	settings.collisionMultiplier = 2.0f;
	// domain
	settings.domainPosition = glm::vec3(0, 2, 0);
	settings.domainSize = glm::vec3(10, 6, 6);
	settings.drawDomain = true;
	settings.drawPartition = false;

	ParticleSystem snow(settings);
	snow.initialise();
	snow.setWsGeometryBuffer(vboGeometry.getTextureGLID(), vboGeometry.getGLID());
	snow.setPointSize(2);
	gui.setSelectedParticleSystem(std::make_shared<ParticleSystem>(snow));

	bool quit = false;
	SceneMode mode = MODE_VIEW;

	float cameraMoveSpeed;
	
	// fps counter variables
	int frames = 0;
	float fps = 0.0f;
	Timer runtime;
	float startTime = runtime.getDuration().count();
	float lastTime = 0;
	float deltaTime = 0;

	CONSOLE_MESSAGE("Scene vertex count: " << vertexCount);
	CONSOLE_MESSAGE("Scene triangle count: " << triangleCount);

	// in-app performance benchmarking
	LogFile		glLogger("benchmarks/GL_Log.txt");
	GPU_Timer	transformFeedbackTimer;

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
		else if (keyboardState[SDL_SCANCODE_LCTRL])
		{
			cameraMoveSpeed = 1;
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
			cameraDataUniformBuffer->loadData(&depthCamera.getCameraUniformData(), 0, sizeof(GPU_UB_CameraData));

			// render all objects
			renderer.renderToDepthBuffer(groundPlane);
			renderer.renderToDepthBuffer(sceneObject);
			renderer.renderToDepthBuffer(sceneObject2);
		}
		renderer.unBindDepthFrameBuffer();

		// switch camera back to main
		Camera::activeCamera->updateCameraUniform();
		cameraDataUniformBuffer->loadData(&Camera::activeCamera->getCameraUniformData(), 0, sizeof(GPU_UB_CameraData));


		// render with transform feedback going to world space geometry texture buffer
		GLuint tf_vao = vaoGeometry.getGLID();
		GLuint tf_vbo = vboGeometry.getGLID();

		glBindVertexArray(tf_vao);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tf_vbo);
			
		groundPlane_COLLISION.m_shader->bind();

		glBeginTransformFeedback(GL_TRIANGLES);
		// begin collision mesh transform feedback
			glEnable(GL_RASTERIZER_DISCARD);

			transformFeedbackTimer.start();

			// table
			sceneObject_COLLISION.m_shader->setUniformMat4f("u_modelMatrix", sceneObject.transform.getModelMatrix());
			renderer.render(sceneObject_COLLISION);
			// bin
			sceneObject2_COLLISION.m_shader->setUniformMat4f("u_modelMatrix", sceneObject2.transform.getModelMatrix());
			renderer.render(sceneObject2_COLLISION);
			// grass
			groundPlane_COLLISION.m_shader->setUniformMat4f("u_modelMatrix", groundPlane.transform.getModelMatrix());
			renderer.render(groundPlane_COLLISION);

			transformFeedbackTimer.end();

			glDisable(GL_RASTERIZER_DISCARD);
		// end collision mesh transform feedback
		glEndTransformFeedback();
			
		// 2nd pass: rendering to frame buffer
		renderer.bindFrameBuffer();
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// render visuals for objects
			// table
			sceneObject.m_shader->setUniformMat4f("u_modelMatrix", sceneObject.transform.getModelMatrix());
			sceneObject.m_shader->setUniformMat4f("u_depthSpaceMatrix", depthCamera.getCameraUniformData().viewProjectionMatrix);
			sceneObject.m_texture->bind(0);
			renderer.render(sceneObject);
			// bin
			sceneObject2.m_shader->setUniformMat4f("u_modelMatrix", sceneObject2.transform.getModelMatrix());
			sceneObject2.m_shader->setUniformMat4f("u_depthSpaceMatrix", depthCamera.getCameraUniformData().viewProjectionMatrix);
			sceneObject2.m_texture->bind(0);
			renderer.render(sceneObject2);
			// ground plane
			groundPlane.m_shader->setUniformMat4f("u_modelMatrix", groundPlane.transform.getModelMatrix());
			groundPlane.m_shader->setUniformMat4f("u_depthSpaceMatrix", depthCamera.getCameraUniformData().viewProjectionMatrix);
			groundPlane.m_texture->bind(0);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			renderer.render(groundPlane);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


			// update and render snow
			if (state.deltaTime < 0.02f)
				snow.updateParticles(state.deltaTime, triangleCount);
		}
		renderer.unBindFrameBuffer();

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

		std::stringstream ss_transformFeedbackTimer;
		ss_transformFeedbackTimer << "Transform Feedback: " << transformFeedbackTimer.getDuration() << "ns";
		glLogger.write(ss_transformFeedbackTimer.str());
	}

	return 0;
}