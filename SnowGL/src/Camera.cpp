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
		m_front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
		m_front.y = sin(glm::radians(m_pitch));
		m_front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

		cleanViewMatrix();
		// grab the new matrices
		m_uniformData.viewMatrix = glm::lookAt(transform.getPosition(), transform.getPosition() + m_front, m_up);
		m_uniformData.projectionMatrix = getProjectionMatrix();
		m_uniformData.orthographicMatrix = getOrthographicMatrix();


	}
}