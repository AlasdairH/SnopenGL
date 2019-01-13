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

		/** @brief Gets the combined model matrix
		*	@return The model matrix
		*
		*	Returns the combined Translation, Rotation and Scale matrix as the model matrix.
		*/
		glm::mat4 getModelMatrix() const;

		/** @brief Translates the transform
		*	@param _tanslation The translation to add to the transform
		*
		*	Translates the transform by the given vec3
		*/
		void translate(const glm::vec3 &_tanslation);

		/** @brief Rotates the transform to a new rotation relative to its old rotation
		*	@param	_rotation	The difference in rotation to apply
		*	@param	_axis		The axis to rotate on
		*
		*	Changes the rotation of the transform by the given vector 3D.
		*/
		void rotate(const float _rotation, const glm::vec3 _axis);

		/** @brief Scales the transform
		*	@param _scale The factors to scale the transform by
		*
		*	Scales the transform by the given vec3
		*/
		void scale(const glm::vec3 &_scale);

	protected:
		glm::mat4 m_mat_position	= glm::mat4(1.0f);	/**< The position as a 4x4 Matrix */
		glm::mat4 m_mat_rotation	= glm::mat4(1.0f);	/**< The rotation as a 4x4 Matrix */
		glm::mat4 m_mat_scale		= glm::mat4(1.0f);	/**< The scale as a 4x4 Matrix */

		glm::vec3 m_vec_position	= glm::vec3(0);		/**< The position as a vec3 */
		glm::vec3 m_vec_scale		= glm::vec3(1);		/**< The scale as a vec3 */
		glm::vec3 m_vec_rotation	= glm::vec3(0);		/**< The rotation as a vec3 */

		glm::mat4 m_modelMatrix		= glm::mat4(1);		/**< The model matrix (pos, rot, scale combined) */
	};
}