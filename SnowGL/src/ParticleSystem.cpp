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
		LOG(LOG_DEBUG) << "Initialising Snowfall Particle System";

		glPointSize(m_pointSize);

		m_tfShader = std::make_shared<ShaderProgram>();
		m_renderShader = std::make_shared<ShaderProgram>();

		// setup drawable domain
		m_drawableDomain = std::make_shared<Renderable>();
		IOUtilities::loadRenderable(*m_drawableDomain, "resources/objects/Cube.rnd");
		m_domainTransform.scale(glm::vec3(m_settings->domainSize.x, m_settings->domainSize.y, m_settings->domainSize.z));

		// set the extents of the domain based on the centre
		glm::vec3 bottomLeft;
		glm::vec3 topRight;
		float minLeftFace = m_settings->domainPosition.x - (m_settings->domainSize.x / 2);
		float minBottomFace = m_settings->domainPosition.y - (m_settings->domainSize.y / 2);
		float minBackFace = m_settings->domainPosition.z - (m_settings->domainSize.z / 2);
		//if (minLeftFace < 0.0f)
			//m_domainOffset.x = minLeftFace * -1;
		//if (minBottomFace < 0.0f)
			//m_domainOffset.y = minBottomFace * -1;
		//if (minBackFace < 0.0f)
			//m_domainOffset.z = minBackFace * -1;

		// create the transform feedback shaders for the particle system
		// vertex
		Shader tfVert(SHADER_VERTEX);
		tfVert.load("resources/shaders/particle/transform_feedback/tf_particle.vert");
		m_tfShader->attachShader(tfVert);
		// fragment
		Shader tfFrag(SHADER_FRAGMENT);
		tfFrag.load("resources/shaders/particle/transform_feedback/tf_particle.frag");
		m_tfShader->attachShader(tfFrag);
		// compile and link
		std::vector<std::string> tfVaryings{ "out_position", "out_startPosition", "out_velocity", "out_mass", "out_startTime", "out_lifetime"};
		m_tfShader->setTransformFeedbackVarying(tfVaryings);
		m_tfShader->link();

		
		// create the rendering shaders for the particle system
		// vertex
		Shader renderVert(SHADER_VERTEX);
		renderVert.load("resources/shaders/particle/render/rnd_particle.vert");
		m_renderShader->attachShader(renderVert);
		// geometry
		Shader renderGeom(SHADER_GEOMETRY);
		renderGeom.load("resources/shaders/particle/render/rnd_particle.geom");
		m_renderShader->attachShader(renderGeom);
		// fragment
		Shader renderFrag(SHADER_FRAGMENT);
		renderFrag.load("resources/shaders/particle/render/rnd_particle.frag");
		m_renderShader->attachShader(renderFrag);
		// compile and link
		m_renderShader->link();
		// load the texture for a snowflake
		m_snowflakeTexture = std::make_shared<Texture>();
		m_snowflakeTexture->load("Snowflake", "resources/textures/snowflake.png");
		

		// get the number of particles required
		m_numParticles = m_settings->getMaxParticles();

		// set the particle buffer layout
		VertexBufferLayout layout;
		layout.push<glm::vec4>(1);	// position (w = is active)
		layout.push<glm::vec4>(1);	// start position
		layout.push<glm::vec3>(1);	// velocity
		layout.push<float>(1);		// mass
		layout.push<float>(1);		// delay to start time
		layout.push<float>(1);		// lifetime

		// create 2 buffers for pingpong
		for (int i = 0; i < 2; ++i)
		{
			// create 2 VAOs and VBOs for ping ponging
			m_tfVAO[i] = std::make_shared<VertexArray>();
			m_tfVBO[i] = std::make_shared<VertexBuffer>(BUFFER_ARRAY);

			m_tfVBO[i]->bind();
			// load correctly sized but empty data to the VBO
			m_tfVBO[i]->loadData(nullptr, sizeof(Particle) * m_numParticles);


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
				buffer[j].mass = Utils::randFloat(0.02f, 0.05f);
				buffer[j].delay = (j / (float)m_numParticles) * m_settings->lifetimeMax;
				buffer[j].lifetime = Utils::randFloat(m_settings->lifetimeMin, m_settings->lifetimeMax);
			}

			glUnmapBuffer(BUFFER_ARRAY);
			

			m_tfVAO[i]->bind();
			m_tfVBO[i]->bind(BUFFER_ARRAY);
			m_tfVAO[i]->addBuffer(*m_tfVBO[i], layout);
		}

		// particle system setup
		applySettingsToShader();

		LOG(LOG_DEBUG) << "Created " << m_numParticles << " particles on the GPU";

		// setup accumulation SSBO
		m_SSBO_AccumulationData.dimensions = glm::vec4(m_settings->domainSize.x, 1.75f, m_settings->domainSize.z, 0);
		m_SSBO_AccumulationData.position = glm::vec4(0, 0.75f, 0, 0);
		int rootPartition = (int)std::floor(std::cbrt(SSBO_MAX_INT_ARRAY));
		m_SSBO_AccumulationData.resolution = glm::vec4((float)rootPartition);
		// pre compute
		m_SSBO_AccumulationData.positionBL = m_SSBO_AccumulationData.position - (m_SSBO_AccumulationData.dimensions / 2.0f);
		m_SSBO_AccumulationData.binSize = m_SSBO_AccumulationData.dimensions / m_SSBO_AccumulationData.resolution;

		m_accumulationSSBO = std::make_shared<VertexBuffer>(BUFFER_SHADER_STORAGE);
		// allocate the memory on the GPU for the partition (data preceding bins + bins)
		m_accumulationSSBO->allocate(sizeof(GPU_SSBO_accumulationPartition) + (sizeof(int) * SSBO_MAX_INT_ARRAY));
		// load the data that precedes the bin array
		m_accumulationSSBO->loadSubData(&m_SSBO_AccumulationData, 0, sizeof(GPU_SSBO_accumulationPartition), 0);
		// link the uniform buffer to the binding point
		m_accumulationSSBO->bindBase(BUFFER_SHADER_STORAGE, SHADER_BINDPOINT_ACCUMULATION_PARTITION);

		LOG(LOG_DEBUG) << "Created buffers for collision data";

		// setup wind field SSBO
		m_SSBO_windFieldData.dimensions = glm::vec4(m_settings->domainSize, 0);
		m_SSBO_windFieldData.position = glm::vec4(m_settings->domainPosition, 1.0f);
		m_SSBO_windFieldData.resolution = glm::vec4(10, 10, 10, 0);
		// pre compute
		m_SSBO_windFieldData.positionBL = m_SSBO_windFieldData.position - (m_SSBO_windFieldData.dimensions / 2.0f);
		m_SSBO_windFieldData.binSize = m_SSBO_windFieldData.dimensions / m_SSBO_windFieldData.resolution;

		m_windFieldSSBO = std::make_shared<VertexBuffer>(BUFFER_SHADER_STORAGE);
		// allocate the memory on the GPU for the partition (data preceding bins + bins)
		int numBins = m_SSBO_windFieldData.resolution.x * m_SSBO_windFieldData.resolution.y * m_SSBO_windFieldData.resolution.z;
		m_windFieldSSBO->allocate(sizeof(GPU_SSBO_accumulationPartition) + (sizeof(glm::vec4) * numBins));
		// load the data that precedes the bin array
		m_windFieldSSBO->loadSubData(&m_SSBO_windFieldData, 0, sizeof(GPU_SSBO_accumulationPartition), 0);
		// link the uniform buffer to the binding point
		m_windFieldSSBO->bindBase(BUFFER_SHADER_STORAGE, SHADER_BINDPOINT_WINDFIELD_PARTITION);
		// bind the buffer to the array buffer binding point
		m_windFieldSSBO->bind(BUFFER_ARRAY);
		// get a pointer to the wind field buffer on the GPU
		glm::vec4 *windFieldBuffer = (glm::vec4 *)glMapBuffer(BUFFER_ARRAY, GL_WRITE_ONLY);
		// offset the pointer to the start of the bin data
		windFieldBuffer += 5;
		// set a range for the random wind value
		float randRange = 0.01f;
		// loop through each bin
		for (int index = 0; index < numBins; ++index)
		{
			windFieldBuffer[index] = glm::vec4(Utils::randFloat(-randRange, randRange), Utils::randFloat(-randRange, randRange), Utils::randFloat(-randRange, randRange), 1);
		}

		glUnmapBuffer(BUFFER_ARRAY);

		LOG(LOG_DEBUG) << "Created buffers for wind field data";

		return true;
	}

	void ParticleSystem::applySettingsToShader()
	{
		// environment
		m_tfShader->setUniform3f("u_globalWind", m_settings->globalWind);
		m_tfShader->setUniform1f("u_collisionMultiplier", m_settings->collisionMultiplier);
		m_tfShader->setUniform3f("u_initialVelocity", m_settings->initialVelocity);
		LOG(LOG_DEBUG) << "Particle settings applied to shader";

		m_domainTransform.setPosition(m_settings->domainPosition);
	}

	void ParticleSystem::updateParticles(float _deltaTime, int _triangleCount)
	{
		
		m_tfShader->bind();
		m_tfShader->setUniform1f("u_deltaTime", _deltaTime);
		m_tfShader->setUniform1f("u_simTime", m_simTime);
		m_tfShader->setUniform1i("u_triangleCount", _triangleCount);

		ApplicationState &state = ApplicationState::getInstance();
		if (!state.isPaused)
		{
			// update the simulation time
			m_simTime += _deltaTime;

			// ping pong - tfb
			m_currentTFBVAO = m_currentTFBVBO;
			m_currentTFBVBO = (m_currentTFBVBO + 1) & 0x1;
			// ping pong - render
			m_currentRenderVAO = m_currentRenderVBO;
			m_currentRenderVBO = (m_currentRenderVBO + 1) & 0x1;

			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_BUFFER, m_wsGeomTextureBuffer);

			m_tfShader->setUniformMat4f("u_modelMatrix", m_transform.getModelMatrix());

			// first pass, particle caculations
			m_tfVAO[m_currentTFBVAO]->bind();
			m_tfVBO[m_currentTFBVBO]->bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0);

			glEnable(GL_RASTERIZER_DISCARD);
			glBeginTransformFeedback(GL_POINTS);
			glDrawArrays(GL_POINTS, 0, m_numParticles);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);
		}

		// 2nd pass, visual render
		m_renderShader->bind();
		m_renderShader->setUniformMat4f("u_modelMatrix", Transform().getModelMatrix());
		m_snowflakeTexture->bind();
		m_tfVAO[m_currentRenderVAO]->bind();

		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_POINTS, 0, m_numParticles);
		glEnable(GL_CULL_FACE);

		++m_frameCount;

		if (m_settings->drawDomain)
		{
			// debug object?
		}
		if (m_settings->drawPartition)
		{
			Debug &debug = Debug::getInstance();
			Transform transform;

			glm::vec3 binSize;
			binSize = m_SSBO_windFieldData.dimensions / m_SSBO_windFieldData.resolution;

			transform.setScale(glm::vec3(binSize.x, binSize.y, binSize.z));

			for (float x = (m_SSBO_windFieldData.position.x - (m_SSBO_windFieldData.dimensions.x / 2.0f)) + binSize.x / 2; 
				x <= (m_SSBO_windFieldData.position.x + (m_SSBO_windFieldData.dimensions.x / 2.0f)) - binSize.x / 2; 
				x += binSize.x)
			{
				for (float y = (m_SSBO_windFieldData.position.y - (m_SSBO_windFieldData.dimensions.y / 2.0f)) + binSize.y / 2; 
					y <= (m_SSBO_windFieldData.position.y + (m_SSBO_windFieldData.dimensions.y / 2.0f)) - binSize.y / 2; 
					y += binSize.y)
				{
					for (float z = (m_SSBO_windFieldData.position.z - (m_SSBO_windFieldData.dimensions.z / 2.0f)) + binSize.z / 2;
						z <= (m_SSBO_windFieldData.position.z + (m_SSBO_windFieldData.dimensions.z / 2.0f)) - binSize.z / 2;
						z += binSize.z)
					{
						transform.setPosition(glm::vec3(x, y, z));
						debug.drawCube(transform, glm::vec4(1.0f, 0.0f, 0.0f, 0.2f));
					}
				}
			}
		}
	}
}