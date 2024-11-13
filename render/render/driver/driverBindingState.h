#pragma once
#include "../../global/base.h"
#include "../../global/constant.h"
#include "../../core/geometry.h"
#include "../../core/object3d.h"
#include "../../core/attribute.h"
#include "../../material/material.h"
#include "driverAttributes.h"
#include "driverPrograms.h"

class DriverBindingStates;
class DriverBindingState {
	friend DriverBindingStates;
public:
	using Ptr = std::shared_ptr<DriverBindingState>;
	static Ptr Create() {
		return std::make_shared<DriverBindingState>();
	}
	DriverBindingState()noexcept;

	~DriverBindingState()noexcept;

private:
	GLuint mVAO{ 0 };
	std::unordered_map<std::string, ID>mAttributes{};
	ID mIndex{ 0 };
	uint32_t mAttributesNum{ 0 };
};

class DriverBindingStates {
public:
	using GeometryKeyMap = std::unordered_map<ID, DriverBindingState::Ptr>;
	using Ptr = std::shared_ptr<DriverBindingStates>;
	static Ptr Create(const DriverAttributes::Ptr& attributes) {
		return std::make_shared<DriverBindingStates>(attributes);
	}
	DriverBindingStates(const DriverAttributes::Ptr& attributes)noexcept;

	~DriverBindingStates()noexcept;

	DriverBindingState::Ptr GetBindingState(const Geometry::Ptr& geometry)noexcept;

	void Setup(const Geometry::Ptr& geometry, const Attributeui::Ptr& index)noexcept;

	DriverBindingState::Ptr CreateBindingState(GLint vao)noexcept;

	bool NeedsUpdate(const Geometry::Ptr& Geometry, const Attributeui::Ptr& index)noexcept;

	void SaveCache(const Geometry::Ptr& Geometry, const Attributeui::Ptr& index)noexcept;

	void SetupVertexAttributes(const Geometry::Ptr& geometry)noexcept;

	GLint CreateVAO()noexcept;

	void BindVAO(GLint vao)noexcept;

	void ReleaseStatesOfGeometry(ID GeometryID)noexcept;

private:
	DriverAttributes::Ptr mAttributes{ nullptr };
	DriverBindingState::Ptr mCurrentBindingState{ nullptr };
	GeometryKeyMap mBindingStates{};
};