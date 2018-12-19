#include "PCH.h"
#include "Camera.h"

namespace SnowGL
{
	Camera *Camera::activeCamera = nullptr;

	Camera::Camera()
	{
		m_fovRad = 0.7f;
		m_aspect = 1.0f;

		init();
	}

	Camera::Camera(const int _resolutionWidth, const int _resolutionHeight)
	{
		m_fovRad = 0.7f;
		setAspectRatio(_resolutionWidth / (float)_resolutionHeight);

		m_viewportWidth = _resolutionWidth;
		m_viewportHeight = _resolutionHeight;

		init();
	}

	void Camera::init()
	{
		// create the uniform buffer
		m_uniformBuffer = std::make_shared<VertexBuffer>(BUFFER_UNIFORM);
		// load the data to the uniform buffer
		m_uniformBuffer->loadData(&m_uniformData, 0, sizeof(m_uniformData));

		// link the uniform buffer to the binding point
		m_uniformBuffer->bindBase(SHADER_BINDPOINT_CAMERA_VP);
		// now the shader and the uniform buffer are pointing at the same binding point the fun can commence

		// if there is no current active camera, set it to this one
		if (Camera::activeCamera == nullptr)
		{
			activeCamera = this;
		}

		cleanViewMatrix();
		updateCameraUniform();
	}

	void Camera::cleanViewMatrix()
	{
		m_projectionMatrix = glm::perspective(m_fovRad, m_aspect, 0.1f, 100.0f);
		m_orthographicMatrix = glm::ortho(0.0f, (float)m_viewportWidth, 0.0f, (float)m_viewportHeight, -1000.0f, 1000.0f);// *glm::lookAt(transform.getPositionVec3(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));;
	}

	void Camera::updateCameraUniform()
	{
		cleanViewMatrix();
		// grab the new matrices
		m_uniformData.viewMatrix = glm::inverse(transform.getModelMatrix());
		m_uniformData.projectionMatrix = getProjectionMatrix();
		m_uniformData.orthographicMatrix = getOrthographicMatrix();

		// bind the uniform buffer
		m_uniformBuffer->bind();
		// load the data to the uniform buffer
		m_uniformBuffer->loadData(&m_uniformData, 0, sizeof(m_uniformData));
	}
}