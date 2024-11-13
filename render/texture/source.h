#pragma once
#include"../global/base.h"
#include"../global/constant.h"

class Source {
public:
	using Ptr = std::shared_ptr<Source>;
	static Ptr Create()
	{
		return std::make_shared<Source>();
	}
	Source()noexcept {};
	~Source()noexcept {};
 
public:
	uint32_t	mWidth{ 0 };
	uint32_t	mHeight{ 0 };
	std::vector<Byte>	mData{};
	bool mNeedsUpdate{ true };

	HashType	mHashCode{ 0 };
	uint32_t	mRefCount{ 0 };
};