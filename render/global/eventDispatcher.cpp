#include "eventDispatcher.h"

EventBase::EventBase(const std::string& eventName) noexcept {
	mEventName = eventName;
}

EventBase::~EventBase() noexcept {}

EventDispatcher* EventDispatcher::mInstance = nullptr;
EventDispatcher* EventDispatcher::getInstance() {
	if (mInstance == nullptr) {
		mInstance = new EventDispatcher();
	}

	return mInstance;
}

EventDispatcher::EventDispatcher() noexcept {}

EventDispatcher::~EventDispatcher() noexcept {}

void EventDispatcher::dispatchEvent(const EventBase::Ptr& event) {
	const auto& eventName = event->mEventName;

	auto listenerQueueIter = mListeners.find(eventName);
	if (listenerQueueIter == mListeners.end()) {
		return;
	}

	auto& queue = listenerQueueIter->second;
	for (const auto& listener : queue) {
		//mFunction���Ѿ�bind��target��function�ĺ�����,����ֱ��ִ��
		listener->mFunction(event);
	}
}

