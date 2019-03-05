#include "PCH.h"
#include "ParticleSystem.h"

namespace SnowGL
{
	ParticleSystem::ParticleSystem(const ParticleSettings &_settings)
	{
		m_settings = std::make_shared<ParticleSettings>(_settings);
	}

	ParticleSystem::~ParticleSystem()
	{

	}

	bool ParticleSystem::initialise()
	{
		CONSOLE_MESSAGE("Initialising Snowfall Particle System");

		glPointSize(m_pointSize);

		m_tfShader = std::make_shared<ShaderProgram>();

		// setup drawable domain
		m_drawableDomain = std::make_shared<Renderable>();
		IOUtilities::loadRenderable(*m_drawableDomain, "resources/objects/Cube.rnd");
		m_partitionPlane = std::make_shared<Renderable>();
		IOUtilities::loadRenderable(*m_partitionPlane, "resources/objects/PartitionPlane.rnd");
		m_domainTransform.scale(glm::vec3(m_settings->domainSize.x, m_settings->domainSize.y, m_settings->domainSize.z));

		glm::vec3 bottomLeft;
		glm::vec3 topRight;
		float minLeftFace = m_settings->domainPosition.x - (m_settings->domainSize.x / 2);
		float minBottomFace = m_settings->domainPosition.y - (m_settings->domainSize.y / 2);
		float minBackFace = m_settings->domainPosition.z - (m_settings->domainSize.z / 2);
		if (minLeftFace < 0.0f)
			m_domainOffset.x = minLeftFace * -1;
		if (minBottomFace < 0.0f)
			m_domainOffset.y = minBottomFace * -1;
		if (minBackFace < 0.0f)
			m_domainOffset.z = minBackFace * -1;

		Shader tfVert(SHADER_VERTEX);
		tfVert.load("resources/shaders/particle/particle.vert");
		m_tfShader->attachShader(tfVert);
		Shader tfFrag(SHADER_FRAGMENT);
		tfFrag.load("resources/shaders/particle/particle.frag");
		m_tfShader->attachShader(tfFrag);
		std::vector<std::string> tfVaryings{ "out_position", "out_startPosition", "out_velocity", "out_startTime", "out_lifetime"};
		m_tfShader->setTransformFeedbackVarying(tfVaryings);
		m_tfShader->link();

		m_numParticles = m_settings->getMaxParticles();

		VertexBufferLayout layout;
		layout.push<glm::vec4>(1);	// position (w = is active)
		layout.push<glm::vec4>(1);	// start position
		layout.push<glm::vec3>(1);	// velocity
		layout.push<float>(1);		// delay
		layout.push<float>(1);		// lifetime


		for (int i = 0; i < 2; ++i)
		{
			// create 2 VAOs and VBOs for ping ponging
			m_tfVAO[i] = std::make_shared<VertexArray>();
			m_tfVBO[i] = std::make_shared<VertexBuffer>(BUFFER_ARRAY);

			m_tfVBO[i]->bind();
			// load correctly sized but empty data to the VBO
			m_tfVBO[i]->loadData(nullptr, sizeof(Particle) * m_numParticles);

			// if the buffer is the first to be created, send it the actual particle data
			if (i == 0)
			{
				// set a pointer to the buffer in GPU memory
				Particle *buffer = (Particle *)glMapBuffer(BUFFER_ARRAY, GL_WRITE_ONLY);

				for (int j = 0; j < m_numParticles; ++j)
				{
					// position w = state
					// -1 = active
					// >= 0 index of last triangle collision
					buffer[j].currentPosition = glm::vec4(Utils::randFloat(m_settings->domainSize.x, -m_settings->domainSize.x) / 2, m_settings->domainSize.y / 2, Utils::randFloat(m_settings->domainSize.z, -m_settings->domainSize.z) / 2, -1) + glm::vec4(m_settings->domainPosition, 0.0f);
					buffer[j].startPosition = buffer[j].currentPosition;
					buffer[j].velocity = m_settings->initialVelocity;
					buffer[j].delay = (j / (float)m_numParticles) * m_settings->lifetimeMax;
					buffer[j].lifetime = Utils::randFloat(m_settings->lifetimeMin, m_settings->lifetimeMax);
				}

				glUnmapBuffer(BUFFER_ARRAY);
			}

			m_tfVAO[i]->bind();
			m_tfVBO[i]->bind(BUFFER_ARRAY);
			m_tfVAO[i]->addBuffer(*m_tfVBO[i], layout);
		}

		// particle system setup
		applySettingsToShader();

		// set current vertex buffer and current transform feedback buffer to be alternate of eachother (0, 1);
		m_currVAO = m_currVBO;
		m_currVBO = (m_currVBO + 1) & 0x1;

		CONSOLE_MESSAGE("Created " << m_numParticles << " particles on the GPU");

		// create a texture buffer for the collision data to be written to
		m_accumulationBufferVBO = std::make_shared<VertexBuffer>(BUFFER_ARRAY);
		m_accumulationBufferVBO->addTextureBuffer(GL_R32I, 1024 * sizeof(int) * 4);
		m_accumulationBufferVAO = std::make_shared<VertexArray>();
		VertexBufferLayout colLayout;
		layout.push<glm::vec4>(1);
		m_accumulationBufferVAO->addBuffer(*m_accumulationBufferVBO, colLayout);


		m_SSBO_AccumulationData.dimensions = glm::vec4(m_settings->domainSize.x, m_settings->domainSize.y, m_settings->domainSize.z, 1.0f);
		m_SSBO_AccumulationData.position = glm::vec4(m_settings->domainPosition, 0.0f);
		m_SSBO_AccumulationData.resolution = glm::vec4(3, 3, 3, 0.0f);

		m_accumulationSSBO = std::make_shared<VertexBuffer>(BUFFER_SHADER_STORAGE);
		// load the data to the uniform buffer
		m_accumulationSSBO->loadData(&m_SSBO_AccumulationData, 0, sizeof(SSBO_accumulationPartition));
		// link the uniform buffer to the binding point
		m_accumulationSSBO->bindBase(BUFFER_SHADER_STORAGE, SHADER_BINDPOINT_ACCUMULATION_PARTITION);

		CONSOLE_MESSAGE("Created buffers for collision data");

		return true;
	}

	void ParticleSystem::applySettingsToShader()
	{
		// colours
		m_tfShader->setUniform4f("u_startColour", m_settings->colourStart);
		m_tfShader->setUniform4f("u_endColour", m_settings->colourEnd);
		m_tfShader->setUniform4f("u_collisionColour", m_settings->collisionDebugColour);
		//
		m_tfShader->setUniform3f("u_globalWind", m_settings->globalWind);
		m_tfShader->setUniform1f("u_collisionMultiplier", m_settings->collisionMultiplier);
		m_tfShader->setUniform3f("u_initialVelocity", m_settings->initialVelocity);
		// domain position / size
		m_tfShader->setUniform3f("u_domainPosition", m_settings->domainPosition);
		m_tfShader->setUniform3f("u_domainOffset", m_domainOffset);
		m_tfShader->setUniform1f("u_domainWidth", m_settings->domainSize.x);
		m_tfShader->setUniform1f("u_domainHeight", m_settings->domainSize.y);
		m_tfShader->setUniform1f("u_domainDepth", m_settings->domainSize.z);
		CONSOLE_MESSAGE("Particle settings applied to shader");

		//m_domainTransform.setPosition(m_settings->domainPosition + m_domainOffset * 2);
		m_domainTransform.setPosition(m_settings->domainPosition);
	}

	void ParticleSystem::updateParticles(float _deltaTime, int _triangleCount)
	{
		m_simTime += _deltaTime;
		m_tfShader->bind();
		m_tfShader->setUniform1f("u_deltaTime", _deltaTime);
		m_tfShader->setUniform1f("u_simTime", m_simTime);
		m_tfShader->setUniform1i("u_triangleCount", _triangleCount);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_BUFFER, m_wsGeomTextureBuffer);

		glBindImageTexture(1, m_accumulationBufferVBO->getTextureGLID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32I);

		m_tfShader->setUniformMat4f("u_modelMatrix", m_transform.getModelMatrix());

		m_tfVAO[m_currVAO]->bind();
		m_tfVBO[m_currVBO]->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

		glBeginTransformFeedback(GL_POINTS);
		glDrawArrays(GL_POINTS, 0, m_numParticles);
		glEndTransformFeedback();

		// ping pong
		m_currVAO = m_currVBO;
		m_currVBO = (m_currVBO + 1) & 0x1;
			
		++m_frameCount;

		if (m_settings->drawDomain)
		{
			/*
			m_drawableDomain->m_shader->setUniformMat4f("u_modelMatrix", m_domainTransform.getModelMatrix());
			m_drawableDomain->m_shader->setUniform1i("u_diffuseTexture", 0);
			m_drawableDomain->m_texture->bind(0);
			
			m_drawableDomain->m_mesh->m_IBO->bind();
			m_drawableDomain->m_mesh->m_VAO->bind();
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, m_drawableDomain->m_mesh->m_IBO->getCount(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			*/
		}
		if (m_settings->drawPartition)
		{
			Transform transform;
			m_partitionPlane->m_shader->setUniform1i("u_useTexture", false);
			m_partitionPlane->m_shader->setUniform1i("u_useSnow", false);

			glDisable(GL_CULL_FACE);

			m_partitionPlane->m_mesh->m_IBO->bind();
			m_partitionPlane->m_mesh->m_VAO->bind();

			m_partitionPlane->m_shader->setUniform4f("u_fragColour", glm::vec4(1.0f, 0.0f, 0.0f, 0.2f));
			glm::vec3 scale = m_domainTransform.getScale();
			transform.scale(glm::vec3(scale.x, 1.0f, scale.z) * 0.9f);

			for (int x = m_SSBO_AccumulationData.position.x - (m_SSBO_AccumulationData.dimensions.x / 2); x < m_SSBO_AccumulationData.dimensions.x; x += m_SSBO_AccumulationData.dimensions.x / m_SSBO_AccumulationData.resolution.x)
			{

			}
			for (int y = m_SSBO_AccumulationData.position.y - (m_SSBO_AccumulationData.dimensions.y / 2); y < m_SSBO_AccumulationData.dimensions.y; y += m_SSBO_AccumulationData.dimensions.y / m_SSBO_AccumulationData.resolution.y)
			{

			}
			for (int y = m_SSBO_AccumulationData.position.y - (m_SSBO_AccumulationData.dimensions.y / 2); y < m_SSBO_AccumulationData.dimensions.y; y += m_SSBO_AccumulationData.dimensions.y / m_SSBO_AccumulationData.resolution.y)
			{
				transform.setPosition(glm::vec3(x, y, z);
				m_partitionPlane->m_shader->setUniformMat4f("u_modelMatrix", transform.getModelMatrix());
				glDrawElements(GL_TRIANGLES, m_partitionPlane->m_mesh->m_IBO->getCount(), GL_UNSIGNED_INT, 0);
			}

			glEnable(GL_CULL_FACE);
		}
		
	}
}