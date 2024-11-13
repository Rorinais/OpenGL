#pragma once
#include "base.h"



class EventBase {
public:
	using Ptr = std::shared_ptr<EventBase>;
	static Ptr create(const std::string& eventName) {
		return std::make_shared <EventBase>(eventName);
	}

	EventBase(const std::string& eventName) noexcept;

	~EventBase() noexcept;

	std::string mEventName;
	void* mTarget = nullptr;
	void* mpUserData{ nullptr };
};

class EventDispatcher {
public:
	template<typename T>
	using TypedFunctionPointer = void(T::*)(const EventBase::Ptr&);

	using EventFunction = std::function<void(const EventBase::Ptr&)>;

	struct Listener {
		using Ptr = std::shared_ptr<Listener>;
		static Ptr create() { return std::make_shared<Listener>(); }

		Listener() noexcept {}

		~Listener() noexcept {
			if (mFuncionPointerDescriptor.mAddr) {
				free(mFuncionPointerDescriptor.mAddr);
			}
		}

		struct FunctionPointerDescriptor {

			void*	mAddr = nullptr;
			size_t	mSize{ 0 };

			bool operator==(const FunctionPointerDescriptor& other) const {
				if (other.mSize != mSize) {
					return false;
				}

				if (memcmp(mAddr, other.mAddr, mSize) != 0) {
					return false;
				}

				return true;
			}
		};

		template<typename T>
		static FunctionPointerDescriptor buildFunctionPointer(TypedFunctionPointer<T> functionPointer) noexcept {

			size_t s = sizeof(TypedFunctionPointer<T>);
			void* p = malloc(s);

			new(p)TypedFunctionPointer<T>(functionPointer);

			Listener::FunctionPointerDescriptor fpd;
			fpd.mAddr = p;
			fpd.mSize = s;

			return fpd;
		}

		//caller 
		void* mTarget = nullptr;//ĳһ�������thisָ��

		//functionPointerDescriptor
		FunctionPointerDescriptor mFuncionPointerDescriptor{};

		//functionPointer
		//�Ժ���ָ��Ĺ�����
		std::function<void(const EventBase::Ptr&)> mFunction = nullptr;
	};

	static bool listenerIsEqual(const Listener::Ptr& l0, const Listener::Ptr& l1) {
		return l0->mTarget == l1->mTarget && l0->mFuncionPointerDescriptor == l1->mFuncionPointerDescriptor;
	}

	using ListenerQueue = std::vector<Listener::Ptr>;

	EventDispatcher() noexcept;

	~EventDispatcher() noexcept;

	static EventDispatcher* getInstance();

	template<typename T>
	void addEventListener(const std::string& name, T* target, TypedFunctionPointer<T> functionPointer) noexcept;

	template<typename T>
	void removeEventListener(const std::string& name, T* target, TypedFunctionPointer<T> functionPointer) noexcept;

	void dispatchEvent(const EventBase::Ptr& event);

protected:
	std::unordered_map<std::string, ListenerQueue> mListeners;
	static EventDispatcher* mInstance;
};

template<typename T>
void EventDispatcher::addEventListener(const std::string& name, T* target, TypedFunctionPointer<T> functionPointer) noexcept {
	auto queueIter = mListeners.find(name);

	if (queueIter == mListeners.end()) {
		queueIter = (mListeners.insert(std::make_pair(name, ListenerQueue()))).first;
	}

	auto& listenerQueue = queueIter->second;

	auto function = std::bind(functionPointer, target, std::placeholders::_1);

	Listener::Ptr listener = Listener::create();
	listener->mTarget = target;

	listener->mFuncionPointerDescriptor = Listener::buildFunctionPointer(functionPointer);
	listener->mFunction = function;

	auto listenerIter = std::find_if(listenerQueue.begin(), listenerQueue.end(),
		[&](const Listener::Ptr& l) {
			return EventDispatcher::listenerIsEqual(listener, l);
		});
	if (listenerIter != listenerQueue.end()) {
		return;
	}

	listenerQueue.push_back(listener);
}

template<typename T>
void EventDispatcher::removeEventListener(const std::string& name, T* target, TypedFunctionPointer<T> functionPointer) noexcept {
	auto queueIter = mListeners.find(name);

	if (queueIter == mListeners.end()) {
		return;
	}

	auto& listenerQueue = queueIter->second;

	auto function = std::bind(functionPointer, target, std::placeholders::_1);
	Listener::Ptr listener = Listener::create();
	listener->mTarget = target;
	listener->mFuncionPointerDescriptor = Listener::buildFunctionPointer(functionPointer);
	listener->mFunction = function;

	auto listenerIter = std::find_if(listenerQueue.begin(), listenerQueue.end(),
		[&](const Listener::Ptr& l) {
			return EventDispatcher::listenerIsEqual(listener, l);
		});
	if (listenerIter != listenerQueue.end()) {
		listenerQueue.erase(listenerIter);
	}
}
