#include"cache.h"
#include"../texture/texture.h"
#include"../texture/cubeTexture.h"

Cache* Cache::mInstance = nullptr;

Cache* Cache::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Cache();
	}
	return mInstance;
}

Cache::Cache()noexcept
{
	EventDispatcher::getInstance()->addEventListener("sourceRelease", this, &Cache::OnSourceRelease);
}

Cache::~Cache()noexcept{}

Source::Ptr Cache::GetSource(const std::string& path)noexcept
{
	std::hash<std::string>hasher;
	auto hashCode = hasher(path);

	Source::Ptr source = nullptr;

	auto iter = mSources.find(hashCode);
	if (iter != mSources.end())
	{
		source = iter->second;
		source->mRefCount++;
	}
	return source;
}

void Cache::CacheSource(const std::string& path, Source::Ptr source)noexcept
{
	std::hash<std::string>hasher;
	auto hashCode = hasher(path);

	auto iter = mSources.find(hashCode);
	if (iter != mSources.end())
	{
		return;
	}
	source->mHashCode = hashCode;
	source->mRefCount++;
	
	mSources.insert(std::make_pair(hashCode, source));
}

void Cache::OnSourceRelease(const EventBase::Ptr& e)
{
	auto source = static_cast<Source*>(e->mTarget);
	auto hashCode = source->mHashCode;

	auto iter = mSources.find(hashCode);
	if(iter==mSources.end())
	{
		return;
	}
	source->mRefCount--;

	if (source->mRefCount==0)
	{
		mSources.erase(iter);
	}

}
