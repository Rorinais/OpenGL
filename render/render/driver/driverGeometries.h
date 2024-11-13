#pragma
#include"../../global/base.h"
#include"../../core/geometry.h"
#include"driverAttributes.h"
#include"driverInfo.h"
#include"driverBindingState.h"

class DriverGeometries {
public:
	using Ptr = std::shared_ptr<DriverGeometries>;
	static Ptr Create(
		const DriverAttributes::Ptr& attrbutes,
		const DriverInfo::Ptr& info,
		const DriverBindingStates::Ptr& bindingStates
	) {
		return std::make_shared<DriverGeometries>(attrbutes, info, bindingStates);
	}
	DriverGeometries(
		const DriverAttributes::Ptr& attributes,
		const DriverInfo::Ptr& info,
		const DriverBindingStates::Ptr& bindingStates
	)noexcept;

	~DriverGeometries()noexcept;

	Geometry::Ptr Get(const Geometry::Ptr& geometry)noexcept;

	void OnGeometryDispose(const EventBase::Ptr& event);

	void Update(const Geometry::Ptr& geometry)noexcept;

private:
	DriverAttributes::Ptr			mAttributes{ nullptr };
	DriverInfo::Ptr					mInfo{ nullptr };
	DriverBindingStates::Ptr mBindingStates{ nullptr };
	std::unordered_map<ID, bool>	mGeometries{};
};