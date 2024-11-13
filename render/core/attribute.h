#pragma once
#include"../global/base.h"
#include "../global/constant.h"
#include "../tools/identity.h"

//每一个VBO都可以存在Attribute中

template<typename T>
class Attribute :public std::enable_shared_from_this<Attribute<T>>
{
public:
	using Ptr = std::shared_ptr<Attribute<T>>;
	static Ptr Create(const std::vector<T>& data, uint32_t itemSize, BufferAllocType bufferAllocType = BufferAllocType::StaticDrawBuffer)
	{
		return std::make_shared<Attribute<T>>(data, itemSize, bufferAllocType);
	}

	Attribute(const std::vector<T>& data, uint32_t itemSize, BufferAllocType bufferAllocType = BufferAllocType::StaticDrawBuffer) noexcept;

	~Attribute() noexcept;
	
	void SetX(const uint32_t& index, T value) noexcept;

	void SetY(const uint32_t& index, T value) noexcept;

	void SetZ(const uint32_t& index, T value) noexcept;

	T GetX(const uint32_t& index) noexcept;

	T GetY(const uint32_t& index) noexcept;

	T GetZ(const uint32_t& index) noexcept;

	ID	GetId() { return mId; }
	std::vector<T>	GetData() { return mData; }
	uint32_t	GetItemSize() { return mItemSize; }
	uint32_t	GetCount() { return mCount; }
	BufferAllocType	GetBufferAllocType() { return mBufferAllocType; }
	DataType	GetDataType(){ return mDataType; }
	bool	GetNeedsUpdata() { return mNeedsUpdata; }
	Range	GetUpdataRange() { return mUpdataRange; }

private:

	ID	mId{ 0 };
	std::vector<T>	mData{};
	uint32_t	mItemSize{ 0 };
	uint32_t	mCount{ 0 };

	BufferAllocType	mBufferAllocType{ BufferAllocType::StaticDrawBuffer };
	DataType	mDataType{ DataType::FloatType };

	bool	mNeedsUpdata{ true };
	Range	mUpdataRange{};
};

using Attributef = Attribute<float>;
using Attributeui = Attribute<uint32_t>;
using Attributevec3 = Attribute<glm::vec3>;

template<typename T>
Attribute<T>::Attribute(const std::vector<T>& data, uint32_t itemSize, BufferAllocType bufferAllocType) noexcept
{
	mId = Identity::generateID();
	mData = data;
	mItemSize = itemSize;
	mCount = static_cast<uint32_t>(mData.size()) / itemSize;
	mBufferAllocType = bufferAllocType;

	mDataType = toDataType<T>();
}

template<typename T>
Attribute<T>::~Attribute()noexcept
{

}

template<typename T>
void Attribute<T>::SetX(const uint32_t& index, T value) noexcept
{
	assert(index < mCount);
	mData[index * mItemSize] = value;
	mNeedsUpdate = true;
}

template<typename T>
void Attribute<T>::SetY(const uint32_t& index, T value) noexcept
{
	assert(index < mCount);
	mData[index * mItemSize+1] = value;
	mNeedsUpdate = true;
}

template<typename T>
void Attribute<T>::SetZ(const uint32_t& index, T value) noexcept
{
	assert(index < mCount);
	mData[index * mItemSize+2] = value;
	mNeedsUpdate = true;
}

template<typename T>
T Attribute<T>::GetX(const uint32_t& index) noexcept
{
	assert(index < mCount);

	return mData[index * mItemSize];
}

template<typename T>
T Attribute<T>::GetY(const uint32_t& index) noexcept
{
	assert(index < mCount);

	return mData[index * mItemSize+1];
}

template<typename T>
T Attribute<T>::GetZ(const uint32_t& index) noexcept
{
	assert(index < mCount);

	return mData[index * mItemSize+2];
}