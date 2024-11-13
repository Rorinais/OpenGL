#include"driverBindingState.h"
#include "../../core/geometry.h"

DriverBindingState::DriverBindingState()noexcept {}

DriverBindingState::~DriverBindingState()noexcept{
	if (mVAO) {
		glDeleteVertexArrays(1, &mVAO);
	}
}

DriverBindingStates::DriverBindingStates(const DriverAttributes::Ptr& attributes)noexcept {
	mAttributes = attributes;
}

DriverBindingStates::~DriverBindingStates()noexcept {}

void DriverBindingStates::Setup(const Geometry::Ptr& geometry, const Attributeui::Ptr& index)noexcept {
	bool updateBufferLayout = false;

	auto state = GetBindingState(geometry);
	if (mCurrentBindingState!=state)
	{
		mCurrentBindingState = state;
		BindVAO(state->mVAO);
	}

	updateBufferLayout = NeedsUpdate(geometry, index);
	if (updateBufferLayout)
	{
		SaveCache(geometry, index);
	}

	if (index!=nullptr)
	{
		mAttributes->Update(index, BufferType::IndexBuffer);
	}

	if (updateBufferLayout)
	{
		SetupVertexAttributes(geometry);
		if (index!=nullptr)
		{
			auto bkIndex = mAttributes->Get(index);
			if (bkIndex!=nullptr)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bkIndex->mHandle);
			}
		}
	}

}

DriverBindingState::Ptr DriverBindingStates::GetBindingState(const Geometry::Ptr& geometry)noexcept {
	DriverBindingState::Ptr state = nullptr;

	auto gKeyIter = mBindingStates.find(geometry->GetId());
	if (gKeyIter == mBindingStates.end())
	{
		gKeyIter = mBindingStates.insert(std::make_pair(geometry->GetId(), CreateBindingState(CreateVAO()))).first;
	}
	state = gKeyIter->second;
	return state;
}

DriverBindingState::Ptr DriverBindingStates::CreateBindingState(GLint vao)noexcept {
	auto bindingState = DriverBindingState::Create();
	bindingState->mVAO = vao;
	return bindingState;
}

GLint DriverBindingStates::CreateVAO()noexcept {
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	return vao;
}

bool DriverBindingStates::NeedsUpdate(const Geometry::Ptr& Geometry, const Attributeui::Ptr& index)noexcept {
	auto cashedAttributes = mCurrentBindingState->mAttributes;
	auto geometryAttributes = Geometry->GetAttributes();

	uint32_t attributesNum = 0;
	for (const auto& iter : geometryAttributes) {
		auto key = iter.first;
		auto geometryAttribute = iter.second;

		auto cachedIter = cashedAttributes.find(key);
		if (cachedIter == cashedAttributes.end()) {
			return true;
		}

		auto cashedAttribute = cachedIter->second;
		if (cashedAttribute!=geometryAttribute->GetId())
		{
			return true;
		}

		attributesNum++;
	}
	if (mCurrentBindingState->mAttributesNum!=attributesNum)
	{
		return true;
	}

	if (index!=nullptr&&mCurrentBindingState->mIndex!=index->GetId())
	{
		return true;
	}
	return false;
}

void DriverBindingStates::SaveCache(const Geometry::Ptr& Geometry, const Attributeui::Ptr& index)noexcept {
	auto& cachedAttributes = mCurrentBindingState->mAttributes;
	cachedAttributes.clear();
	auto attributes = Geometry->GetAttributes();
	uint32_t attributesNum = 0;

	for (const auto&iter:attributes)
	{
		auto attribute = iter.second;
		cachedAttributes.insert(std::make_pair(iter.first, attribute->GetId()));
		attributesNum++;
	}
	mCurrentBindingState->mAttributesNum = attributesNum;
	
	if (index!=nullptr)
	{
		mCurrentBindingState->mIndex = index->GetId();
	}
}

void DriverBindingStates::SetupVertexAttributes(const Geometry::Ptr& geometry)noexcept {
	const auto geometryAttributes = geometry->GetAttributes();
	
	for (const auto&iter:geometryAttributes)
	{
		auto name = iter.first;
		auto attribute = iter.second;
		auto itemSize = attribute->GetItemSize();
		auto dataType = attribute->GetDataType();
		auto bkAttribute = mAttributes->Get(attribute);

		auto bindingIter = LOCATION_MAP.find(name);
		if (bindingIter==LOCATION_MAP.end())
		{
			continue;
		}
		auto binding = bindingIter->second;

		glBindBuffer(GL_ARRAY_BUFFER, bkAttribute->mHandle);
		glEnableVertexAttribArray(binding);
		glVertexAttribPointer(binding, itemSize, toGL(dataType), false, itemSize * toSize(dataType), (void*)0);
	}


}

void DriverBindingStates::BindVAO(GLint vao)noexcept {
	glBindVertexArray(vao);
}

void DriverBindingStates::ReleaseStatesOfGeometry(ID GeometryID)noexcept {
	auto iter = mBindingStates.find(GeometryID);
	if (iter != mBindingStates.end()) {
		mBindingStates.erase(iter);
	}
}

