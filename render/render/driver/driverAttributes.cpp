#include "driverAttributes.h"


DriverAttribute::DriverAttribute() noexcept {}

DriverAttribute::~DriverAttribute() noexcept {
	if (mHandle) {
		glDeleteBuffers(1, &mHandle);
	}
}

DriverAttributes::DriverAttributes() noexcept {
	EventDispatcher::getInstance()->addEventListener("attributeDispose", this, &DriverAttributes::OnAttributeDispose);
}

DriverAttributes::~DriverAttributes() noexcept {
	EventDispatcher::getInstance()->removeEventListener("attributeDispose", this, &DriverAttributes::OnAttributeDispose);
}

void DriverAttributes::Remove(ID attributeID) noexcept {
	auto iter = mAttributes.find(attributeID);
	if (iter != mAttributes.end()) {
		mAttributes.erase(iter);
	}
}

void DriverAttributes::OnAttributeDispose(const EventBase::Ptr& e) {
	ID attrID = *((ID*)e->mpUserData);
	Remove(attrID);
}
