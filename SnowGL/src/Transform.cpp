#include "PCH.h"
#include "Transform.h"

namespace SnowGL
{
	glm::mat4 Transform::getModelMatrix() const
	{
		glm::mat4 modelMatrix = m_mat_position * m_mat_rotation * m_mat_scale;
		return modelMatrix;
	}

	void Transform::translate(const glm::vec3 & _tanslation)
	{
		m_vec_position += _tanslation;
		m_mat_position = glm::translate(m_mat_position, _tanslation);
	}

	void Transform::rotate(const float _deg, const glm::vec3 _axis)
	{
		if (_axis.x)
		{
			m_vec_rotation.x += _deg;
		}
		if (_axis.y)
		{
			m_vec_rotation.y += _deg;
		}
		if (_axis.z)
		{
			m_vec_rotation.z += _deg;
		}

		m_mat_rotation = glm::rotate(m_mat_rotation, glm::radians(_deg), _axis);
	}

	void Transform::scale(const glm::vec3 & _scale)
	{
		m_vec_scale += _scale;
		m_mat_scale = glm::scale(m_mat_scale, _scale);
	}
}