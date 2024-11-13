#include"driverObjects.h"

DriverObjects::DriverObjects(
	const DriverGeometries::Ptr& geometries,
	const DriverAttributes::Ptr& attributes,
	const DriverInfo::Ptr& info
)noexcept {
	mGeometries = geometries;
	mAttributes = attributes;
	mInfo = info;
}

DriverObjects::~DriverObjects()noexcept {}

Geometry::Ptr DriverObjects::Update(const RenderableObject::Ptr& object)noexcept {
	const auto frame = mInfo->mRender.mFrame;
	const auto geometry = mGeometries->Get(object->GetGeometry());

	bool needsUpdate = false;

	auto iter = mUpdateMap.find(geometry->GetId());

	if (iter == mUpdateMap.end() || mUpdateMap[geometry->GetId()] != frame)
	{
		mGeometries->Update(geometry);
		mUpdateMap[geometry->GetId()] = frame;
	}
	return geometry;
}