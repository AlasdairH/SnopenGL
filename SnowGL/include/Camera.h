#pragma once

// cstdlib


// external libs

// program
#include "PCH.h"
#include "VertexBuffer.h"
#include "Transform.h"

// TODO: Matrix Optimisation
namespace SnowGL
{
	/*! @class u_CameraData
	*	@brief The CPU side uniform block for camera view and projection matrices
	*
	*	This data corresponds to the shader side "u_camera_data" for the per frame unchanging data of camera matrices.
	*/
	struct u_CameraData
	{
		glm::mat4 viewMatrix;			/**< Camera View Matrix data */
		glm::mat4 projectionMatrix;		/**< Camera Projection Matrix data */
		glm::mat4 orthographicMatrix;	/**< Camera Projection Matrix data */
	};


	/*! @class Camera
	*	@brief Contains the data for the representation of a perspective camera
	*
	*	Contains the data required to represent a camera in OpenGL. This uses the glm::perspective function to create a camera that can be moved around using the
	*	transform.
	*/
	class Camera : public std::enable_shared_from_this<Camera>
	{
	public:
		/** @brief Camera Ctor
		*
		*	Creates a camera at the world 0, 0, 0 with a standard FoV
		*/
		Camera();
		/** @brief Camera Ctor
		*	@param _resolutionWidth The width of the camera's resolution
		*	@param _resolutionHeight The height of the camera's resolution
		*
		*	Creates a camera at the world 0, 0, 0 with a standard FoV at the specified resolution
		*/
		Camera(const int _resolutionWidth, const int _resolutionHeight);
		/** @brief Camera Dtor
		*
		*	Removes the camera
		*/
		~Camera() { }

		/** @brief Sets the active camera
		*
		*	Sets the active camera in the engine to this camera
		*/
		inline void setActive() { activeCamera = this; }

		Transform transform;		/**< The transform attribute of the Camera, used for moving it around the world */

		/** @brief Sets the Field of View
		*	@param _fov The desired Field of View in Radians
		*
		*	Sets the FoV and cleans the projection matrix
		*/
		inline void setFOV(const float _fov) { m_fovRad = _fov; cleanViewMatrix(); }
		/** @brief Sets the Aspect Ratio ( width / height )
		*	@param _aspect The desired Aspect Ratio
		*
		*	Sets the Aspect Ratio and cleans the projection matrix
		*/
		inline void setAspectRatio(const float _aspect) { m_aspect = _aspect; cleanViewMatrix(); }

		/** @brief Return the Projection Matrix
		*	@return The Projection Matrix
		*
		*	Returns the Projection Matrix
		*/
		inline glm::mat4 getProjectionMatrix() { return m_projectionMatrix; }

		/** @brief Return the Orthographic Matrix
		*	@return The Orthographic Matrix
		*
		*	Returns the Orthographic Matrix
		*/
		inline glm::mat4 getOrthographicMatrix() { return m_orthographicMatrix; }

		/** @brief Updates the OpenGL Camera Data Uniform
		*
		*	Updates the GPU side uniform for camera view and projection matrices.
		*/
		void updateCameraUniform();

		/** @brief Returns the width of the viewport
		*	@return The width of the viewport
		*
		*	Returns the camera's viewport width
		*/
		inline int getViewportWidth() { return m_viewportWidth; }
		/** @brief Returns the height of the viewport
		*	@return The height of the viewport
		*
		*	Returns the camera's viewport height
		*/
		inline int getViewportHeight() { return m_viewportHeight; }

		/** @brief Sets the camera front 
		*	@param _front The new camera front value
		*
		*	Sets the new front orientation for the camera
		*/
		inline void setFront(glm::vec3 _front) { m_front = _front; }

		static Camera *activeCamera;	/**< The currently active camera */

	protected:
		/** @brief Initialises the camera data after the constructor
		*
		*	Initialises the camera data after the constructor
		*/
		void init();
		/** @brief Sets the updated Projection Matrix
		*
		*	Sets a fresh perspective matrix from the FoV, aspect ratio and near / far planes
		*/
		void cleanViewMatrix();

		float										m_fovRad;						/**< The current FoV in radians */
		float										m_aspect;						/**< The current Aspect Ratio */
		int											m_viewportWidth;				/**< Width of the viewport in pixels */
		int											m_viewportHeight;				/**< Height of the viewport in pixels */

		glm::vec3									m_up = glm::vec3(0, 1, 0);
		glm::vec3									m_front = glm::vec3(0, 0, -1);

		glm::mat4									m_projectionMatrix;				/**< The Projection Matrix */
		glm::mat4									m_orthographicMatrix;			/**< The Orthographic Matrix */

		std::shared_ptr<VertexBuffer>				m_uniformBuffer;				/**< The uniform buffer for View and Projection matrices */
		u_CameraData								m_uniformData;					/**< The struct containing the matrices for the uniform buffer to use */
	};
}
