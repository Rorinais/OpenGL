#include"../core/object3d.h"

class Group :public Object3d
{
public:
	using Ptr = std::shared_ptr<Group>;
	static Ptr Create()
	{
		return std::make_shared<Group>();
	}

	Group()noexcept
	{
		mIsGroup = true;
	}

	~Group()noexcept{}

	uint32_t mGroupOrder{ 0 };
};