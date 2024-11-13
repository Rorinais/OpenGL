#pragma once
#include "../../global/base.h"
#include "../../core/attribute.h"
#include "../../global/eventDispatcher.h"

class DriverAttribute {
public:
	using Ptr = std::shared_ptr<DriverAttribute>;
	static Ptr Crate() {
		return std::make_shared<DriverAttribute>();
	}
	DriverAttribute()noexcept;

	~DriverAttribute()noexcept;

	//VBO,vertexBufferObject
	GLuint mHandle{ 0 };
};

class DriverAttributes {
public:
	using Ptr = std::shared_ptr<DriverAttributes>;
	static Ptr Create() {
		return std::make_shared<DriverAttributes>();
	}

	using DriverAttributesMap = std::unordered_map<ID, DriverAttribute::Ptr>;
	DriverAttributes()noexcept;
	~DriverAttributes()noexcept;
	
	template<typename T>
	DriverAttribute::Ptr Get(const std::shared_ptr<Attribute<T>>& attribute)noexcept;

	template<typename T>
	DriverAttribute::Ptr Update(
		const std::shared_ptr<Attribute<T>>&attribute,
		const BufferType&bufferType	)noexcept;

	void Remove(ID attrributeID) noexcept;

	void OnAttributeDispose(const EventBase::Ptr& e);

private:
	DriverAttributesMap mAttributes{};
};

template<typename T>
DriverAttribute::Ptr DriverAttributes::Get(const std::shared_ptr<Attribute<T>>& attribute) noexcept {
	auto iter = mAttributes.find(attribute->GetId());
	if (iter != mAttributes.end()) {
		return iter->second;
	}
	return nullptr;
}

template <typename T>
DriverAttribute::Ptr DriverAttributes::Update(
	const std::shared_ptr<Attribute<T>>& attribute,
	const BufferType& bufferType) noexcept{
	DriverAttribute::Ptr dattribute = nullptr;

	auto iter = mAttributes.find(attribute->GetId());
	if (iter!=mAttributes.end())
	{
		dattribute = iter->second;
	}
	else
	{
		dattribute = DriverAttribute::Crate();
		auto data = attribute->GetData();

		glGenBuffers(1, &dattribute->mHandle);
		glBindBuffer(toGL(bufferType), dattribute->mHandle);
		glBufferData(toGL(bufferType), data.size() * sizeof(T), data.data(), toGL(attribute->GetBufferAllocType()));
		glBindBuffer(toGL(bufferType), 0);

		mAttributes.insert(std::make_pair(attribute->GetId(), dattribute));

		attribute->ClearUpdateRange();
		attribute->ClearNeedsUpdate();
	}
	if (attribute->GetNeedsUpdate())
	{
		attribute->ClearNeedsUpate();

		auto updateRange = attribute->getUpdateRange();
		auto data = attribute->GetData();

		glBindBuffer(toGL(bufferType), dattribute->mHandle);
		if (updateRange.mCount > 0) {
			glBufferSubData(
				toGL(bufferType),
				updateRange.mOffset * sizeof(T),
				updateRange.mCount * sizeof(T),
				data.data() + updateRange.mOffset * sizeof(T));
		}
		else
		{
			glBufferData(toGL(bufferType), data.size() * sizeof(T), data.data(), toGL(attribute->GetBufferAllocType()));
		}
		glBindBuffer(toGL(bufferType), 0);

		attribute->ClearUpdateRange();
	}
	return dattribute;
}