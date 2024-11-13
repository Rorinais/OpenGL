#include "perspectiveCamera.h"

namespace ff {

	PerspectiveCamera::PerspectiveCamera(float near, float far, float aspect, float fov) noexcept {
		mIsPerpectiveCamera = true;
		mNear = near;
		mFar = far;
		mAspect = aspect;
		mFov = fov;

		UpdateProjectionMatrix();
	}

	PerspectiveCamera::~PerspectiveCamera() noexcept {}

	glm::mat4 PerspectiveCamera::UpdateProjectionMatrix() noexcept {
		mProjectionMatrix = glm::perspective(glm::radians(mFov), mAspect, mNear, mFar);

		return mProjectionMatrix;
	}
}