#pragma once

// TODO: Doxygen

// cstdlib

// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	class Transform
	{
	public:
		Transform();

		glm::mat4 getModelMatrix() const;

		void translate(const glm::vec3 &_tanslation);
		/** @brief Rotates the transform to a new rotation relative to its old rotation
		*	@param	_rotation	The difference in rotation to apply
		*	@param	_axis		The axis to rotate on
		*
		*	Changes the rotation of the transform by the given vector 3D.
		*/
		void rotate(const float _rotation, const glm::vec3 _axis);
		void scale(const glm::vec3 &_scale);

	protected:
		glm::mat4 m_mat_position	= glm::mat4(1.0f);
		glm::mat4 m_mat_rotation	= glm::mat4(1.0f);
		glm::mat4 m_mat_scale		= glm::mat4(1.0f);

		glm::vec3 m_vec_position	= glm::vec3(0);
		glm::vec3 m_vec_scale		= glm::vec3(1);
		glm::vec3 m_vec_rotation	= glm::vec3(0);

		glm::mat4 m_modelMatrix		= glm::mat4(1);
	};
}