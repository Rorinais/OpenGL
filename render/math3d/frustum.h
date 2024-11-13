#pragma once 
#include "../global/base.h"
#include "plane.h"
#include "sphere.h"
#include "../object/renderableObject.h"

class Frustum
{
public:
	using Ptr = std::shared_ptr<Frustum>;
	static Ptr Create()
	{
		return std::make_shared<Frustum>();
	}

	Frustum()noexcept
	{
		mToolSphere = Sphere::Create(glm::vec3(0.0f), 0.0f);
		for (uint32_t i = 0; i < 6; ++i)
		{
			mPlanes.push_back(Plane::Create(glm::vec3(0.0f), 0.0f));
		}
	}

	void SetFormProjectionMatrix(const glm::mat4& matrix)noexcept
	{
		auto m = glm::value_ptr(matrix);
		mPlanes[0]->SetComponents(m[3] - m[0], m[7] - m[4], m[11] - m[8], m[15] - m[12]);
		mPlanes[1]->SetComponents(m[3] + m[0], m[7] + m[4], m[11] + m[8], m[15] + m[12]);
		mPlanes[2]->SetComponents(m[3] + m[1], m[7] + m[5], m[11] + m[9], m[15] + m[13]);
		mPlanes[3]->SetComponents(m[3] - m[1], m[7] - m[5], m[11] - m[9], m[15] - m[13]);
		mPlanes[4]->SetComponents(m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);
		mPlanes[5]->SetComponents(m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14]);
	}

	bool IntersectObject(const RenderableObject::Ptr & object) noexcept {
		auto geometry = object->GetGeometry();

		if (geometry->GetBoundingSphere() == nullptr) {
			geometry->ComputeBoundingSphere();
		}

		mToolSphere->Copy(geometry->GetBoundingSphere());
		mToolSphere->ApplyMatrix4(object->GetWorldMatrix());

		return IntersectSphere(mToolSphere);
	}

	bool IntersectSphere(const Sphere::Ptr & sphere) noexcept {
		auto center = sphere->mCenter;
		auto radius = sphere->mRadius;

		for (uint32_t i = 0; i < 6; ++i) {
			auto distance = mPlanes[i]->DistanceToPoint(center);

			if (distance < -radius) {
				return false;
			}
		}
		return true;
	}

private:
	std::vector<Plane::Ptr> mPlanes{};
	Sphere::Ptr mToolSphere{ nullptr };
};