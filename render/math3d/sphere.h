#pragma once
#include"../global/base.h"

class Sphere
{
public:
	using Ptr = std::shared_ptr<Sphere>;

	static Ptr Create(const glm::vec3& center, float randius)
	{
		return std::make_shared<Sphere>(center, randius);
	}

	Sphere(const glm::vec3& center, float radius)noexcept
	{
		mCenter = center;
		mRadius = radius;
	}
	~Sphere()noexcept{}

	void ApplyMatrix4(const glm::mat4 matrix)noexcept
	{
		mCenter = glm::vec3(matrix * glm::vec4(mCenter, 1.0));
		float scaleX = glm::length(glm::vec3(matrix[0]));
		float scaleY = glm::length(glm::vec3(matrix[1]));
		float scaleZ = glm::length(glm::vec3(matrix[2]));

		float maxScale = std::max(std::max(scaleX, scaleY), scaleZ);
		mRadius = maxScale;
	}
	void Copy(const Sphere::Ptr& other)
	{
		mCenter = other->mCenter;
		mRadius = other->mRadius;
	}

public:
	glm::vec3 mCenter{ 0 };
	float mRadius{ 0 };
};