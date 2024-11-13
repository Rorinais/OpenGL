#pragma once
#include"../../global/base.h"

class DriverInfo {
public:
	struct Memory{
		uint32_t mGeometries{ 0 };
		uint32_t mTextures{ 0 };
	};

	struct Render {
		uint32_t mFrame{ 0 };
		uint32_t mCalls{ 0 };
		uint32_t mTriangels{ 0 };
	};

	using Ptr = std::shared_ptr<DriverInfo>;
	static Ptr Create(){
		return std::make_shared<DriverInfo>();
	}

	DriverInfo()noexcept;

	~DriverInfo()noexcept;

	void Update(uint32_t count, uint32_t glMode, uint32_t instanceCout)noexcept;

	void Reset() noexcept;

public:
	Memory mMemory{};
	Render mRender{};
};