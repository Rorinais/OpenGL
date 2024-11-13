#pragma once
#include"../global/base.h"
#include"attribute.h"
#include"../math3d/box3.h"
#include"../math3d/sphere.h"
//Geometry相当于VAO,所有的几何数据存在AttributeMap里，Index存在Attributeui::Ptr里，
// 可以通过SetAttribute和GetAttribute设置和获取几何数据，通过GetAttributes获取全部几何数据
//通过SetIndex和GetIndex设置和获取顶点索引


class Geometry :public std::enable_shared_from_this<Geometry>
{
public:
	using AttributeMap = std::unordered_map<std::string, Attributef::Ptr>;

	using Ptr = std::shared_ptr<Geometry>;
	static Ptr Create()
	{
		return std::make_shared<Geometry>();
	}
	Geometry() noexcept;
	~Geometry() noexcept;

	void SetAttribute(const std::string& name, Attributef::Ptr attribute)noexcept;

	Attributef::Ptr GetAttribute(const std::string& name) noexcept;

	void DeleteAttribute(const std::string& name)noexcept;

	bool hasAttribute(const std::string name)noexcept;

	const AttributeMap& GetAttributes()const noexcept;

	ID GetId() const noexcept;

	void SetIndex(const Attributeui::Ptr& index)noexcept;

	auto GetIndex()const noexcept { return mIndexAttrbute; }

	void ComputeBoundingBox()noexcept;
	void ComputeBoundingSphere()noexcept;
	Box3::Ptr	GetBoundingBox()const noexcept { return mBoundingBox; }
	Sphere::Ptr	GetBoundingSphere()const noexcept { return mBoundingSphere; }

private:
	ID	mId{ 0 };
	AttributeMap	mAttributes{};
	Attributeui::Ptr	mIndexAttrbute{ nullptr };

	Box3::Ptr	mBoundingBox{ nullptr };
	Sphere::Ptr	mBoundingSphere{ nullptr };
};