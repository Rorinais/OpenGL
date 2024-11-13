#include"material.h"
#include"../global/eventDispatcher.h"
#include"../tools/identity.h"

Material::Material()noexcept
{
	mId = Identity::generateID();
	mType = MaterialName::Material;
	mIsMaterial = true;
}
Material::~Material()noexcept {
	EventBase::Ptr e = EventBase::create("materialDispose");
	e->mTarget = this;
	EventDispatcher::getInstance()->dispatchEvent(e);
}