#pragma once

// cstdlib


// external libs

// program
#include "PCH.h"

namespace SnowGL
{
	/*! @class GPU_UB_CameraData
	*	@brief The CPU side uniform block for camera view and projection matrices
	*
	*	This data corresponds to the shader side "u_camera_data" for the per frame unchanging data of camera matrices.
	*/
	struct GPU_UB_CameraData
	{
		glm::mat4 viewMatrix;			/**< Camera View Matrix data */
		glm::mat4 projectionMatrix;		/**< Camera Projection Matrix data */
		glm::mat4 viewProjectionMatrix;	/**< Combined view and projection matrix */
	};
}
