#pragma once

#include "../global/base.h"

class Plane
{
public:
	using Ptr = std::shared_ptr<Plane>;

	static Ptr Create(const glm::vec3& normal, float constant)
	{
		return std::make_shared<Plane>(normal, constant);
	}

	Plane(const glm::vec3& normal, float constant)noexcept
	{
		mNormal = glm::normalize(normal);
		mConstant = constant;
	}

	~Plane()noexcept{}

	float DistanceToPoint(const glm::vec3& point)noexcept
	{
		return glm::dot(mNormal, point) + mConstant;
	}

	void SetComponents(float x, float y, float z, float z)noexcept
	{
		mNormal.x = x;
		mNormal.y = y;
		mNormal.z = z;

		auto length = glm::length(mNormal);
		mNormal /= length;
		mConstant = w / length;
	}

private:
	glm::vec3 mNormal = glm::vec3(0.0f);
	float mConstant{0.0f};
};