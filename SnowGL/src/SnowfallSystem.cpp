#include "PCH.h"
#include "..\include\SnowfallSystem.h"

namespace SnowGL
{
	SnowfallSystem::SnowfallSystem()
	{

	}

	SnowfallSystem::~SnowfallSystem()
	{

	}
	bool SnowfallSystem::initialise()
	{
		m_tfShader = std::make_shared<ShaderProgram>();

		Shader tfVert(SHADER_VERTEX);
		tfVert.load("resources/shaders/particle/particle.vert");
		m_tfShader->attachShader(tfVert);
		std::vector<std::string> tfVaryings{ "out_position", "out_velocity", "out_lifetime" };
		m_tfShader->setTransformFeedbackVarying(tfVaryings);
		m_tfShader->link();

		std::vector<Particle> data;
		data.push_back(Particle { glm::vec3(1, 2, 3), glm::vec3(0, 0, 0), 10 } );
		data.push_back(Particle { glm::vec3(2, 3, 4), glm::vec3(0, 0, 0), 20 } );
		data.push_back(Particle { glm::vec3(3, 4, 5), glm::vec3(0, 0, 0), 30 } );

		m_numParticles = data.size();

		VertexBufferLayout layout;
		layout.push<glm::vec3>(1);	// position
		layout.push<glm::vec3>(1);	// velocity
		layout.push<float>(1);		// lifetime

		// Create VAO
		m_tfVAO = std::make_shared<VertexArray>();

		// Create transform feedback buffer 0 and 1
		m_tfBuffer[0] = std::make_shared<VertexBuffer>(BUFFER_ARRAY);
		m_tfBuffer[1] = std::make_shared<VertexBuffer>(BUFFER_TRANSFORM_FEEDBACK);

		// load initial data to buffer 1
		m_tfBuffer[0]->loadData(data.data(), sizeof(Particle) * data.size());
		m_tfBuffer[1]->loadData(nullptr, sizeof(Particle) * data.size());

		// set attrib array in VAO
		m_tfVAO->addBuffer(*m_tfBuffer[0], layout);

		// set current vertex buffer and current transform feedback buffer to be alternate of eachother (0, 1);
		//m_currVB = m_currTFB;
		//m_currTFB = (m_currTFB + 1) & 0x1;

		return true;
	}

	void SnowfallSystem::render(int _deltaTime, const glm::mat4 &_VP, const glm::vec3 &_cameraPos)
	{

		updateParticles(_deltaTime);

		renderParticles(_VP, _cameraPos);

		m_currVB = m_currTFB;
		m_currTFB = (m_currTFB + 1) & 0x1;
	}

	void SnowfallSystem::renderParticles(const glm::mat4 & _VP, const glm::vec3 & _cameraPos)
	{

	}

	void SnowfallSystem::updateParticles(int _deltaTime)
	{
		// Perform feedback transform
		glEnable(GL_RASTERIZER_DISCARD);

		m_tfBuffer[1]->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

		glBeginTransformFeedback(GL_POINTS);
		glDrawArrays(GL_POINTS, 0, m_numParticles);
		glEndTransformFeedback();

		glDisable(GL_RASTERIZER_DISCARD);

		glFlush();

		// Fetch and print results
		std::vector<Particle> particles;
		particles.resize(m_numParticles);

		glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particles.size() * sizeof(Particle), particles.data());

		for (int i = 0; i < particles.size(); ++i)
		{
		CONSOLE_MESSAGE(particles[i].position.x << ", " << particles[i].position.y << ", " << particles[i].position.z << " | "
			<< particles[i].velocity.x << ", " << particles[i].velocity.y << ", " << particles[i].velocity.z << " | " << particles[i].lifetime);
		}
	}
}