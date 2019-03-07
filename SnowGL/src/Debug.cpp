#include "PCH.h"
#include "Debug.h"

namespace SnowGL
{
	Debug::Debug()
	{
		m_debugPlane = std::make_shared<Renderable>();
		IOUtilities::loadRenderable(*m_debugPlane, "resources/objects/PartitionPlane.rnd");

		m_debugCube = std::make_shared<Renderable>();
		IOUtilities::loadRenderable(*m_debugCube, "resources/objects/Cube.rnd");
	}

	Debug::~Debug()
	{

	}

	void Debug::drawCube(Transform &_transform, glm::vec4 _colour)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);

		// bind vao / ibo
		m_debugCube->m_mesh->m_IBO->bind();
		m_debugCube->m_mesh->m_VAO->bind();

		// setup shader
		m_debugCube->m_shader->setUniformMat4f("u_modelMatrix", _transform.getModelMatrix());
		m_debugCube->m_shader->setUniform1i("u_useTexture", false);
		m_debugCube->m_shader->setUniform1i("u_useSnow", false);
		m_debugCube->m_shader->setUniform4f("u_fragColour", _colour);

		// draw
		glDrawElements(GL_TRIANGLES, m_debugCube->m_mesh->m_IBO->getCount(), GL_UNSIGNED_INT, 0);

		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	void Debug::drawPlane(Transform &_transform, glm::vec4 _colour)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);

		// bind vao / ibo
		m_debugPlane->m_mesh->m_IBO->bind();
		m_debugPlane->m_mesh->m_VAO->bind();

		// setup shader
		m_debugPlane->m_shader->setUniformMat4f("u_modelMatrix", _transform.getModelMatrix());
		m_debugPlane->m_shader->setUniform1i("u_useTexture", false);
		m_debugPlane->m_shader->setUniform1i("u_useSnow", false);
		m_debugPlane->m_shader->setUniform4f("u_fragColour", _colour);

		// draw
		glDrawElements(GL_TRIANGLES, m_debugPlane->m_mesh->m_IBO->getCount(), GL_UNSIGNED_INT, 0);

		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}