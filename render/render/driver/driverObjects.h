#pragma once
#include "../../global/base.h"
#include "../../object/renderableObject.h"
#include "driverGeometries.h"
#include "driverAttributes.h"
#include "driverInfo.h"


class DriverObjects {
public:
	using Ptr = std::shared_ptr<DriverObjects>;
	static Ptr Create(
		const DriverGeometries::Ptr &geometries,
		const DriverAttributes::Ptr &attributes,
		const DriverInfo::Ptr&info
	) {
		return std::make_shared<DriverObjects>(geometries, attributes, info);
	}

	DriverObjects(
		const DriverGeometries::Ptr& geometries,
		const DriverAttributes::Ptr& attributes,
		const DriverInfo::Ptr& info
	)noexcept;

	~DriverObjects()noexcept;

	Geometry::Ptr Update(const RenderableObject::Ptr& object)noexcept;

private:
	std::unordered_map<ID, uint32_t>mUpdateMap{};

	DriverInfo::Ptr		mInfo{ nullptr };

	DriverAttributes::Ptr mAttributes{ nullptr };
	DriverGeometries::Ptr mGeometries{ nullptr };
};