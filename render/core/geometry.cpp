
#include"geometry.h"
#include"../math3d/box3.h"
#include"../math3d/sphere.h"

Geometry::Geometry() noexcept
{
	mId = Identity::generateID();
}
Geometry::~Geometry() noexcept
{

}

void Geometry::SetAttribute(const std::string& name, Attributef::Ptr attribute)noexcept
{
	mAttributes[name] = attribute;
}

Attributef::Ptr Geometry::GetAttribute(const std::string& name) noexcept
{
	auto iter = mAttributes.find(name);
	if (iter != mAttributes.end())
	{
		return iter->second;
	}
	return nullptr;
}

void Geometry::DeleteAttribute(const std::string& name)noexcept
{
	auto iter = mAttributes.find(name);
	if (iter != mAttributes.end())
	{
		mAttributes.erase(iter);
	}
}

bool Geometry::hasAttribute(const std::string name)noexcept
{
	auto iter = mAttributes.find(name);
	if (iter != mAttributes.end())
	{
		return false;
	}
	return true;
}

const Geometry::AttributeMap& Geometry::GetAttributes() const noexcept {
	return mAttributes;
}

ID Geometry::GetId() const noexcept {
	return mId;
}

void Geometry::SetIndex(const Attributeui::Ptr& index)noexcept
{
	mIndexAttrbute = index;
}

void Geometry::ComputeBoundingBox()noexcept
{
	auto position = GetAttribute("position");
	if (position == nullptr)
	{
		std::cout << "Error:: geometry has no position when ComputeBoundingBox" << std::endl;
		return;
	}
	if (mBoundingBox == nullptr)
	{
		mBoundingBox = Box3::Create();

	}
	mBoundingBox->SetFromAttribute(position);
}

void Geometry::ComputeBoundingSphere()noexcept
{
	ComputeBoundingBox();
	if (mBoundingSphere == nullptr)
	{
		mBoundingSphere = Sphere::Create(glm::vec3(0.0f), 0.0f);
	}
	mBoundingSphere->mCenter = mBoundingBox->GetCenter();

	auto position = GetAttribute("position");
	if (position == nullptr)
	{
		return;
	}
	float maxRandusSq = 0;
	for (uint32_t i = 0; i < position->GetCount(); ++i)
	{
		glm::vec3 point = glm::vec3(position->GetX(i), position->GetY(i), position->GetZ(i));
		glm::vec3 randiusVector = mBoundingSphere->mCenter - point;
		maxRandusSq = std::max(glm::dot(randiusVector, randiusVector), maxRandusSq);

	}
	mBoundingSphere->mRadius = std::sqrt(maxRandusSq);
}