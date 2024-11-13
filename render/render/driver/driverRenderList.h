#pragma once
#include"../../global/base.h"
#include"../../object/renderableObject.h"
#include"../../core/geometry.h"
#include"../../material/material.h"

class RenderItem
{
public:
	using Ptr = std::shared_ptr<RenderItem>;
	static Ptr Create()
	{
		return std::make_shared<RenderItem>();
	}

	RenderItem()noexcept;

	~RenderItem()noexcept;

public:
	ID	mId{ 0 };
	float	mZ=0;
	RenderableObject::Ptr	mObject{ nullptr };
	Material::Ptr	mMaterial{ nullptr };
	Geometry::Ptr	mGeometry{ nullptr };
	uint32_t	mGroupOrder{ 0 };
};

using RenderListSortFunction = std::function<bool(const RenderItem::Ptr&, const RenderItem::Ptr&)>;

static bool SmallerZFirstSort(const RenderItem::Ptr& item0, const RenderItem::Ptr& item1)
{
	if (item0->mGroupOrder > item1->mGroupOrder)
	{
		return item0->mGroupOrder > item1->mGroupOrder;
	}
	else if(item0->mZ!=item1->mZ)
	{
		return item0->mZ < item1->mZ;
	}
	else
	{
		return item0->mId > item1->mId;
	}
}

static bool BiggerZFirstSort(const RenderItem::Ptr& item0, const RenderItem::Ptr& item1)
{
	if (item0->mGroupOrder!=item1->mGroupOrder)
	{
		return item0->mGroupOrder > item1->mGroupOrder;
	}
	else if (item0->mZ != item1->mZ)
	{
		return item0->mZ >item1->mZ;
	}
	else
	{
		return item0->mId > item1->mId;
	}
}

class DriverRenderList
{
public:
	using Ptr = std::shared_ptr<DriverRenderList>;
	static Ptr Create()
	{
		return std::make_shared<DriverRenderList>();
	}

	DriverRenderList()noexcept;

	~DriverRenderList()noexcept;

	void Init()noexcept;

	void Push(
		const RenderableObject::Ptr& object,
		const Geometry::Ptr& geometry,
		const Material::Ptr&material,
		const uint32_t&groupOrder,
		float z)noexcept;

	void Sort(
		const RenderListSortFunction& opaqueSort= SmallerZFirstSort,
		const RenderListSortFunction& transparentSort = BiggerZFirstSort
	)noexcept;

	void Finish()noexcept;

	const auto& GetOpaques()const noexcept { return mOpaques; }

	const auto& GetTransparents()const noexcept { return mTransparents; }

private:
	RenderItem::Ptr GetNextRenderItem(
		const RenderableObject::Ptr&object,
		const Geometry::Ptr&geometry,
		const Material::Ptr&material,
		const uint32_t& groupOrder,
		float z)noexcept;
private:
	uint32_t	mRenderItemIndex{ 0 };
	std::vector<RenderItem::Ptr>	mOpaques{};
	std::vector<RenderItem::Ptr>	mTransparents{};
	std::vector<RenderItem::Ptr>	mRenderItemCache{};
};

