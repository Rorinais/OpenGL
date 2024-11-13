#pragma once
#include"../global/base.h"
#include"../global/constant.h"

class Loader
{
public:
	using Ptr = std::shared_ptr<Loader>;

	Loader()noexcept;
	~Loader()noexcept;

};