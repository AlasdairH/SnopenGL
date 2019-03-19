#pragma once

// cstdlib


// external libs

// program
#include "PCH.h"
#include "GPU_UB_CameraData.h"
#include "VertexBuffer.h"
#include "Transform.h"

// TODO: Matrix Optimisation
namespace SnowGL
{
	enum ProjectionMode { PROJECTION_PERSPECTIVE, PROJECTION_ORTHOGRAPHIC };

	/*! @class Camera
	*	@brief Contains the data for the representation of a perspective camera
	*
	*	Contains the data required to represent a camera in OpenGL. This uses the glm::perspective function to create a camera that can be moved around using the
	*	transform.
	*/
	class Camera
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
		*	@param _projection The projection mode of the camera, orthographic or perspective
		*
		*	Creates a camera at the world 0, 0, 0 with a standard FoV at the specified resolution
		*/
		Camera(const int _resolutionWidth, const int _resolutionHeight, ProjectionMode _projection);
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
		glm::mat4 getProjectionMatrix();

		/** @brief Updates the OpenGL Camera Data Uniform
		*
		*	Updates the GPU side uniform for camera view and projection matrices.
		*/
		void updateCameraUniform();

		/** @brief Returns a unifom block struct with the cameras data
		*
		*	Returns a unifom block struct with the cameras data
		*/
		inline GPU_UB_CameraData getCameraUniformData() { return m_uniformData; }

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

		/** @brief Gets the camera front 
		*	@return The front value of the camera
		*
		*	Gets the front orientation for the camera
		*/
		inline glm::vec3 getFront() { return m_front; }

		// TODO: Doxygen
		inline void setProjectionMode(const ProjectionMode _mode) { m_projectionMode = _mode; }
		inline ProjectionMode getProjectionMode() { return m_projectionMode; }

		inline void setPitch(const float &_pitch) { m_pitch = _pitch; }
		inline void setYaw(const float &_yaw) { m_yaw = _yaw; }

		inline float getPitch() { return m_pitch; }
		inline float getYaw() { return m_yaw; }

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

		ProjectionMode								m_projectionMode = PROJECTION_PERSPECTIVE;

		float										m_fovRad;						/**< The current FoV in radians */
		float										m_aspect;						/**< The current Aspect Ratio */
		int											m_viewportWidth;				/**< Width of the viewport in pixels */
		int											m_viewportHeight;				/**< Height of the viewport in pixels */

		float										m_pitch = 0.0f;
		float										m_yaw = -90.0f;

		glm::vec3									m_up = glm::vec3(0, 1, 0);
		glm::vec3									m_front = glm::vec3(0, 0, -1);

		glm::mat4									m_perspectiveProjectionMatrix;				/**< The Perspective Matrix */
		glm::mat4									m_orthographicProjectionMatrix;				/**< The Orthographic Matrix */

		GPU_UB_CameraData								m_uniformData;					/**< The struct containing the matrices for the uniform buffer to use */
	};
}
