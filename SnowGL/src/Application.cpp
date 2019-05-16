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
#include "FrameBenchmark.h"

#undef main

using namespace SnowGL;

int main()
{
	srand(1);

	ApplicationState &state = ApplicationState::getInstance();

	InitManager::initSDL();

	Window window("SnopenGL");

	// initalise OpenGL 
	InitManager::initOpenGL();

	// create the main scene camera
	Camera camera((int)state.windowSize.x, (int)state.windowSize.y, PROJECTION_PERSPECTIVE);
	// translate to the starting position
	camera.transform.translate(glm::vec3(0, 3, 18));

	// create a camera for rendering the depth buffer used to generate shadows
	Camera depthCamera(1024, 1024, PROJECTION_ORTHOGRAPHIC);
	// transform to an above position 
	depthCamera.transform.translate(glm::vec3(0, 5, 0));
	// set the camera looking downwards
	depthCamera.setPitch(-89.9f);

	// create a camera data uniform buffer
	std::shared_ptr<VertexBuffer> cameraDataUniformBuffer = std::make_shared<VertexBuffer>(BUFFER_UNIFORM);
	// link the uniform buffer to the binding point
	cameraDataUniformBuffer->bindBase(GL_UNIFORM_BUFFER, SHADER_BINDPOINT_CAMERA_VP);
	// load the data to the uniform buffer
	cameraDataUniformBuffer->loadData(&camera.getCameraUniformData(), 0, sizeof(GPU_UB_CameraData));

	// create a transform for the centre of the scene
	Transform zeroTransform;

	// ------------------------------------------------------------ 3D Scene ------------------------------------------------------------
	// create a ground plane (grass)
	Renderable groundPlane;
	IOUtilities::loadRenderable(groundPlane, "resources/objects/Plane.rnd");
	Renderable groundPlane_COLLISION;
	IOUtilities::loadRenderable(groundPlane_COLLISION, "resources/objects/Plane_Collision.rnd");

	// create a scene object (park bench)
	Renderable sceneObject_parkBench;
	IOUtilities::loadRenderable(sceneObject_parkBench, "resources/objects/Table.rnd");
	sceneObject_parkBench.transform.translate(glm::vec3(1.5f, 0, 0));
	sceneObject_parkBench.transform.rotate(-45, glm::vec3(0, 1, 0));
	sceneObject_parkBench.m_shader->setUniform1i("u_useSnowTexture", 1);
	sceneObject_parkBench.m_shader->setUniform1i("u_useSnowOffset", 1);
	Renderable sceneObject_parkBench_COLLISION;
	IOUtilities::loadRenderable(sceneObject_parkBench_COLLISION, "resources/objects/Table_Collision.rnd");
	// 2nd park bench (only transform required, will draw from the first bench shader and model)
	Transform secondBench;
	secondBench.setPosition(glm::vec3(-2.5f, 0, 1.5f));
	// create a scene object (bin)
	Renderable sceneObject_bin;
	IOUtilities::loadRenderable(sceneObject_bin, "resources/objects/Bin.rnd");
	sceneObject_bin.transform.translate(glm::vec3(-1, 0, 0));
	sceneObject_bin.m_shader->setUniform1i("u_useSnow", 1);
	sceneObject_bin.m_shader->setUniform1i("u_useSnowOffset", 1);
	Renderable sceneObject_bin_COLLISION;
	IOUtilities::loadRenderable(sceneObject_bin_COLLISION, "resources/objects/Bin_Collision.rnd");	
	// create a scene object (signpost)
	Renderable sceneObject_sign;
	IOUtilities::loadRenderable(sceneObject_sign, "resources/objects/Sign.rnd");
	sceneObject_sign.transform.translate(glm::vec3(-2.5, 0, -1.5f));
	sceneObject_sign.transform.setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	sceneObject_sign.m_shader->setUniform1i("u_useSnow", 1);
	sceneObject_sign.m_shader->setUniform1i("u_useSnowOffset", 1);
	Renderable sceneObject_sign_COLLISION;
	IOUtilities::loadRenderable(sceneObject_sign_COLLISION, "resources/objects/Sign_Collision.rnd");

	// create a texture buffer to contain collidable world space geometry
	VertexBuffer vboGeometry(BUFFER_ARRAY);
	vboGeometry.addTextureBuffer(GL_RGBA32F, 1024 * 1024 * sizeof(glm::vec4));
	VertexArray vaoGeometry;
	VertexBufferLayout layout;
	layout.push<glm::vec4>(1);
	vaoGeometry.addBuffer(vboGeometry, layout);

	int vertexCount = 0;
	vertexCount += groundPlane_COLLISION.getVertexCount();
	vertexCount += sceneObject_parkBench_COLLISION.getVertexCount();
	vertexCount += sceneObject_parkBench_COLLISION.getVertexCount();
	vertexCount += sceneObject_sign_COLLISION.getVertexCount();
	vertexCount += sceneObject_bin_COLLISION.getVertexCount();

#ifdef ENABLE_BENCHMARK
	// create multiple instances of a scene object to test performance
	vertexCount += (sceneObject_COLLISION.getVertexCount() * (COLLISION_BENCHMARK_ITERATIONS - 1));
#endif

	int triangleCount = vertexCount / 3;

	Renderer renderer;

	GUI gui(window.getWindowPtr());

	ParticleSettings settings;
	settings.fromSettingsFile("ParticleSettings.ini");

	ParticleSystem snow(settings);
	snow.initialise();
	snow.setWsGeometryBuffer(vboGeometry.getTextureGLID(), vboGeometry.getGLID());
	gui.setSelectedParticleSystem(std::make_shared<ParticleSystem>(snow));

	SceneMode mode = MODE_VIEW;

	float cameraMoveSpeed;
	
	// fps counter variables
	int frames = 0;
	float fps = 0.0f;
	Timer runtime;
	float startTime = runtime.getDuration().count();
	float lastTime = 0;
	float deltaTime = 0;

	LOG(LOG_DEBUG) << "Scene vertex count: " << vertexCount;
	LOG(LOG_DEBUG) << "Scene triangle count: " << triangleCount;

#ifdef ENABLE_BENCHMARK
	// in-app performance benchmarking
	LogFile			glLogger("benchmarks/GL_Log.txt", LOG_TEXT, LOG_APPEND);
	LogFile			glLoggerCSV("benchmarks/GL_LogCSV.csv", LOG_CSV, LOG_APPEND);
	LogFile			glLoggerOverallCSV("benchmarks/GL_LogCSV_Settings.csv", LOG_CSV, LOG_APPEND);
	FrameBenchmark	frameBenchmark;

	int benchmarkStartFrame = 0;
	bool firstBenchmarkFrame = true;

	float benchmarkStartTime = 10.0f;
	float benchmarkEndTime = 15.0f;

	SDL_GL_SetSwapInterval(!SDL_GL_GetSwapInterval());


	float avgParticleSimTime = 0;
	float avgFPS = 0;

	glLoggerCSV.write(frameBenchmark.getCSVHeaders().str());
#endif

	while (state.isRunning)
	{
		// calculate FPS
		float timepassed = runtime.getDuration().count();
		state.currentTime = timepassed;
		frames++;
		if (timepassed - startTime > 0.25f && frames > 10)
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
		++state.currentFrame;
#ifdef ENABLE_BENCHMARK
		frameBenchmark.frame = state.currentFrame;
#endif

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
		// increase camera move speed on shift hold
		if (keyboardState[SDL_SCANCODE_LSHIFT])
		{
			cameraMoveSpeed = 10;
		}
		// decrease camera move speed on ctrl hold
		else if (keyboardState[SDL_SCANCODE_LCTRL])
		{
			cameraMoveSpeed = 1;
		}
		// default camera move speed
		else
		{
			cameraMoveSpeed = 5;
		}
		// forward
		if (keyboardState[SDL_SCANCODE_W]) 
		{
			Camera::activeCamera->transform.translate(camera.getFront() * cameraMoveSpeed * state.deltaTime);
		}
		// back
		if (keyboardState[SDL_SCANCODE_S])
		{
			Camera::activeCamera->transform.translate(-camera.getFront() * cameraMoveSpeed * state.deltaTime);
		}
		// left
		if (keyboardState[SDL_SCANCODE_A])
		{
			glm::vec3 leftVector = glm::cross(glm::vec3(0, 1, 0), camera.getFront());
			Camera::activeCamera->transform.translate(leftVector * cameraMoveSpeed * state.deltaTime);
		}
		// right
		if (keyboardState[SDL_SCANCODE_D])
		{
			glm::vec3 rightVector = glm::cross(camera.getFront(), glm::vec3(0, 1, 0));
			Camera::activeCamera->transform.translate(rightVector * cameraMoveSpeed * state.deltaTime);
		}		
		// up
		if (keyboardState[SDL_SCANCODE_E])
		{
			Camera::activeCamera->transform.translate(glm::vec3(0, 1, 0) * cameraMoveSpeed * state.deltaTime);
		}
		// down
		if (keyboardState[SDL_SCANCODE_Q])
		{
			Camera::activeCamera->transform.translate(glm::vec3(0, -1, 0) * cameraMoveSpeed * state.deltaTime);
		}

		while (SDL_PollEvent(&incomingEvent))
		{
			// if close button pressed
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

#ifdef ENABLE_BENCHMARK
		frameBenchmark.shadowMapping.start();
#endif
		// 1st pass: render to depth FBO
		renderer.bindDepthFrameBuffer();
		{

			glClear(GL_DEPTH_BUFFER_BIT);

			depthCamera.updateCameraUniform();
			cameraDataUniformBuffer->loadData(&depthCamera.getCameraUniformData(), 0, sizeof(GPU_UB_CameraData));

			// render all objects
			renderer.renderToDepthBuffer(groundPlane);
			renderer.renderToDepthBuffer(sceneObject_parkBench);
			renderer.renderToDepthBuffer(sceneObject_parkBench, secondBench);
			renderer.renderToDepthBuffer(sceneObject_sign);
			renderer.renderToDepthBuffer(sceneObject_bin);
		}
		renderer.unBindDepthFrameBuffer();
#ifdef ENABLE_BENCHMARK
		frameBenchmark.shadowMapping.end();
#endif

		// switch camera back to main
		camera.updateCameraUniform();
		cameraDataUniformBuffer->loadData(&Camera::activeCamera->getCameraUniformData(), 0, sizeof(GPU_UB_CameraData));

#ifdef ENABLE_BENCHMARK
		frameBenchmark.collisionDetectionTransformFeedback.start();
#endif
		// render with transform feedback going to world space geometry texture buffer
		GLuint tf_vao = vaoGeometry.getGLID();
		GLuint tf_vbo = vboGeometry.getGLID();

		glBindVertexArray(tf_vao);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tf_vbo);
			
		groundPlane_COLLISION.m_shader->bind();

		// 2nd pass: collision geometry transform feedback
		glBeginTransformFeedback(GL_TRIANGLES);
		// begin collision mesh transform feedback
			glEnable(GL_RASTERIZER_DISCARD);
			// table
#ifdef ENABLE_BENCHMARK
			// draw multiple for performance testing
			for (int i = 0; i < COLLISION_BENCHMARK_ITERATIONS; ++i)
			{
				sceneObject_COLLISION.m_shader->setUniformMat4f("u_modelMatrix", sceneObject.transform.getModelMatrix());
				renderer.render(sceneObject_COLLISION);
			}
#else
			// table
			sceneObject_parkBench_COLLISION.m_shader->setUniformMat4f("u_modelMatrix", sceneObject_parkBench.transform.getModelMatrix());
			renderer.render(sceneObject_parkBench_COLLISION);
			// table 2
			sceneObject_parkBench_COLLISION.m_shader->setUniformMat4f("u_modelMatrix", secondBench.getModelMatrix());
			renderer.render(sceneObject_parkBench_COLLISION);
#endif
			// sign
			sceneObject_sign_COLLISION.m_shader->setUniformMat4f("u_modelMatrix", sceneObject_sign.transform.getModelMatrix());
			renderer.render(sceneObject_sign_COLLISION);
			// bin
			sceneObject_bin_COLLISION.m_shader->setUniformMat4f("u_modelMatrix", sceneObject_bin.transform.getModelMatrix());
			renderer.render(sceneObject_bin_COLLISION);
			// grass
			groundPlane_COLLISION.m_shader->setUniformMat4f("u_modelMatrix", groundPlane.transform.getModelMatrix());
			renderer.render(groundPlane_COLLISION);

			glDisable(GL_RASTERIZER_DISCARD);
		// end collision mesh transform feedback
		glEndTransformFeedback();

#ifdef ENABLE_BENCHMARK
		frameBenchmark.collisionDetectionTransformFeedback.end();

		frameBenchmark.visuals.start();
#endif
			
		// 3nd pass: rendering to frame buffer
		renderer.bindFrameBuffer();
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// render visuals for objects
			// table
			sceneObject_parkBench.m_shader->setUniformMat4f("u_modelMatrix", sceneObject_parkBench.transform.getModelMatrix());
			sceneObject_parkBench.m_shader->setUniformMat4f("u_depthSpaceMatrix", depthCamera.getCameraUniformData().viewProjectionMatrix);
			sceneObject_parkBench.m_texture->bind(0);
			renderer.render(sceneObject_parkBench);
			// table 2
			sceneObject_parkBench.m_shader->setUniformMat4f("u_modelMatrix", secondBench.getModelMatrix());
			renderer.render(sceneObject_parkBench);
			// bin
			sceneObject_bin.m_shader->setUniformMat4f("u_modelMatrix", sceneObject_bin.transform.getModelMatrix());
			sceneObject_bin.m_shader->setUniformMat4f("u_depthSpaceMatrix", depthCamera.getCameraUniformData().viewProjectionMatrix);
			sceneObject_bin.m_texture->bind(0);
			renderer.render(sceneObject_bin);			
			// sign
			sceneObject_sign.m_shader->setUniformMat4f("u_modelMatrix", sceneObject_sign.transform.getModelMatrix());
			sceneObject_sign.m_shader->setUniformMat4f("u_depthSpaceMatrix", depthCamera.getCameraUniformData().viewProjectionMatrix);
			sceneObject_sign.m_texture->bind(0);
			renderer.render(sceneObject_sign);
			// ground plane
			groundPlane.m_shader->setUniformMat4f("u_modelMatrix", groundPlane.transform.getModelMatrix());
			groundPlane.m_shader->setUniformMat4f("u_depthSpaceMatrix", depthCamera.getCameraUniformData().viewProjectionMatrix);
			groundPlane.m_texture->bind(0);
			
			renderer.render(groundPlane);


#ifdef ENABLE_BENCHMARK
			frameBenchmark.visuals.end();
			frameBenchmark.particleSimulation.start();
#endif

			// update and render snow
			if (runtime.getDuration().count() > 3.0f && state.isRenderingParticles)
				snow.updateParticles(state.deltaTime, triangleCount);

#ifdef ENABLE_BENCHMARK
			frameBenchmark.particleSimulation.end();
#endif
		}
		renderer.unBindFrameBuffer();

		renderer.drawFrameBuffer();
		
		// GUI
		gui.onUpdate();
		gui.onRender();

		window.swapBuffer();

#ifdef ENABLE_BENCHMARK
		if (state.currentTime > benchmarkStartTime && state.currentTime < benchmarkEndTime)
		{
			if (firstBenchmarkFrame == true)
			{
				LOG(LOG_DEBUG) << "Starting Benchmark";
				benchmarkStartFrame = state.currentFrame;
				firstBenchmarkFrame = false;
			}

			glLogger.write(frameBenchmark.getDataSS().str());
			glLoggerCSV.write(frameBenchmark.getDataCSV().str());

			avgParticleSimTime += frameBenchmark.particleSimulation.getDuration() / 1000000;
			avgFPS += state.framesPerSecond;
		}
		// automatically close application after 15 seconds
		if (state.currentTime >= benchmarkEndTime)
		{
			SceneDump dump;
			dump.particleCount = settings.getMaxParticles();
			dump.triangleCount = triangleCount;
			dump.simulatedFrames = state.currentFrame - benchmarkStartFrame;
			dump.avgParticleSimTime = avgParticleSimTime / dump.simulatedFrames;
			dump.avgFPS = avgFPS / dump.simulatedFrames;

			//glLoggerOverallCSV.write(dump.getCSVHeaders().str());
			glLoggerOverallCSV.write(dump.getDataCSV().str());

			state.isRunning = false;
		}
#endif
	}

	return 0;
}