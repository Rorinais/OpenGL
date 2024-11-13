#pragma once
#include"../global/base.h"
#include"../global/constant.h"
#include"../texture/source.h"
#include"../global/eventDispatcher.h"

class Cache
{
public:
	static Cache* GetInstance();

	Cache()noexcept;

	~Cache()noexcept;

	Source::Ptr GetSource(const std::string& path)noexcept;

	void CacheSource(const std::string& path, Source::Ptr source)noexcept;

	void OnSourceRelease(const EventBase::Ptr& e);

private:
	static Cache* mInstance;

	std::unordered_map<HashType, Source::Ptr>mSources{};
};