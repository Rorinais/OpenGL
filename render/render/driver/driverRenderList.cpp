#include"driverRenderList.h"

RenderItem::RenderItem()noexcept{}

RenderItem::~RenderItem()noexcept{}

DriverRenderList::DriverRenderList()noexcept{}

DriverRenderList::~DriverRenderList()noexcept{}

void DriverRenderList::Init()noexcept
{
	mRenderItemIndex = 0;
	mOpaques.clear();
	mTransparents.clear();
}

void DriverRenderList::Push(
	const RenderableObject::Ptr& object,
	const Geometry::Ptr& geometry,
	const Material::Ptr& material,
	const uint32_t& groupOrder,
	float z)noexcept{
	const auto renderItem = GetNextRenderItem(object,geometry,material,groupOrder,z);

	if (material->mTransparent)
	{
		mTransparents.push_back(renderItem);
	}
	else
	{
		mOpaques.push_back(renderItem);
	}
}

void DriverRenderList::Sort(
	const RenderListSortFunction& opaqueSort,
	const RenderListSortFunction& transparentSort)noexcept {

	if (!mOpaques.empty())std::sort(mOpaques.begin(), mOpaques.end(), opaqueSort);

	if (!mTransparents.empty())std::sort(mTransparents.begin(),mTransparents.end(),transparentSort);
}

void DriverRenderList::Finish()noexcept 
{
	auto listSize = mRenderItemCache.size();
	for (uint32_t i = mRenderItemIndex; i < listSize; ++i)
	{
		auto renderItem = mRenderItemCache[i];
		if (renderItem == nullptr)
		{
			break;
		}
		renderItem->mId = 0;
		renderItem->mObject = nullptr;
		renderItem->mGeometry = nullptr;
		renderItem->mMaterial = nullptr;
		renderItem->mGroupOrder = 0;
		renderItem->mZ = 0;
	}

}

RenderItem::Ptr DriverRenderList::GetNextRenderItem(
	const RenderableObject::Ptr& object,
	const Geometry::Ptr& geometry,
	const Material::Ptr& material,
	const uint32_t& groupOrder,
	float z)noexcept {

	RenderItem::Ptr renderItem = nullptr;

	if (mRenderItemIndex >= mRenderItemCache.size())
	{
		renderItem = RenderItem::Create();
		mRenderItemCache.push_back(renderItem);
	}
	else
	{
		renderItem = mRenderItemCache[mRenderItemIndex];
	}
	renderItem->mId = object->GetId();
	renderItem->mObject = object;
	renderItem->mGeometry = geometry;
	renderItem->mMaterial = material;
	renderItem->mGroupOrder = groupOrder;
	renderItem->mZ = z;

	mRenderItemIndex++;

	return renderItem;
}


