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

	ShaderProgram tfShader;

	Shader tfVertexShader(SHADER_VERTEX);
	tfVertexShader.load("resources/shaders/particle/particle.vert");
	tfShader.attachShader(tfVertexShader);

	tfShader.setTransformFeedbackVarying("out_Value", 1);

	tfShader.link();

	VertexArray dataVAO;
	VertexBuffer dataVBO(BUFFER_ARRAY);
	GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
	dataVBO.loadData(data, sizeof(data));
	GLint inputAttrib = tfShader.getAttributeLocation("in_Value");
	dataVAO.setAttribArray(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);


	// create VBO for processed data and give it no data except the size
	VertexBuffer tfDataVBO(BUFFER_ARRAY);
	tfDataVBO.loadData(nullptr, -1, sizeof(data));

	// create mesh
	Mesh mesh;
	IOUtilities::loadMesh(mesh, "resources/models/deer.obj");
	GPU_Mesh openGLMesh;
	openGLMesh.setMesh(mesh);

	// create transform
	Transform transform;
	transform.translate(glm::vec3(0, 0, -8));

	Renderer renderer;

	
	/*
	// Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	GLint inputAttrib = glGetAttribLocation(tfShader.getProgramID(), "in_Value");
	glEnableVertexAttribArray(inputAttrib);
	glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

	// Create transform feedback buffer
	GLuint tbo;
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_READ);

	// Perform feedback transform
	glEnable(GL_RASTERIZER_DISCARD);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, 5);
	glEndTransformFeedback();

	glDisable(GL_RASTERIZER_DISCARD);

	glFlush();
	*/
	// Fetch and print results
	GLfloat feedback[5];
	tfDataVBO.bind();
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

	CONSOLE_MESSAGE(feedback[0] << ", " << feedback[1] << ", " << feedback[2] << ", " << feedback[3] << ", " << feedback[4]);
	
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