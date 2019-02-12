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
		m_perspectiveProjectionMatrix = glm::perspective(m_fovRad, m_aspect, 0.1f, 100.0f);
		float orthoHeight = glm::degrees(m_fovRad) / 4;
		float orthoWidth = orthoHeight;
		m_orthographicProjectionMatrix = glm::ortho(-(orthoWidth / 2), orthoWidth / 2, -(orthoHeight / 2), orthoHeight / 2, 0.0f, 10.0f);
	}

	glm::mat4 Camera::getProjectionMatrix()
	{
		if (m_projectionMode == PROJECTION_PERSPECTIVE)
		{
			
			return m_perspectiveProjectionMatrix;
		}
		else if (m_projectionMode == PROJECTION_ORTHOGRAPHIC)
		{
			return m_orthographicProjectionMatrix;
		}
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
	}
}