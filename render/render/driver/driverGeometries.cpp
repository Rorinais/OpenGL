#include "driverGeometries.h"
#include "../../global/eventDispatcher.h"

DriverGeometries::DriverGeometries(
	const DriverAttributes::Ptr& attributes,
	const DriverInfo::Ptr& info,
	const DriverBindingStates::Ptr& bindingStates
)noexcept {
	mAttributes = attributes;
	mInfo = info;
	mBindingStates = bindingStates;

	EventDispatcher::getInstance()->addEventListener("geometryDispose", this, &DriverGeometries::OnGeometryDispose);
}

DriverGeometries::~DriverGeometries()noexcept {
	EventDispatcher::getInstance()->removeEventListener("geometryDispose", this, &DriverGeometries::OnGeometryDispose);
}

Geometry::Ptr DriverGeometries::Get(const Geometry::Ptr& geometry)noexcept {
	auto iter = mGeometries.find(geometry->GetId());

	if (iter!=mGeometries.end())
	{
		if (iter->second == true) {
			return geometry;
		}
	}

	mGeometries[geometry->GetId()] = true;

	mInfo->mMemory.mGeometries++;

	return geometry;
}

void DriverGeometries::OnGeometryDispose(const EventBase::Ptr& event) {

}

void DriverGeometries::Update(const Geometry::Ptr& geometry)noexcept {
	const auto geometryAttributes = geometry->GetAttributes();
	for (const auto&iter:geometryAttributes)
	{
		mAttributes->Update(iter.second, BufferType::ArrayBuffer);
	}
}