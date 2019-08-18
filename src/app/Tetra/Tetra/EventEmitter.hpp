#pragma once

#include <forward_list>
#include <functional>


// Id is just for uniqueness
template<auto Id, typename... Args>
class EventEmitter {
public:
  using EventListener = std::function<void(Args... args)>;

private:
  std::forward_list<EventListener> mEventListeners{};

protected:
  void DispatchEvent(Args... args) const {
    for (const auto& eventListener : mEventListeners) {
      eventListener(args...);
    }
  }

public:
  void AddEventListener(EventListener eventListener) {
    mEventListeners.push_front(eventListener);
  }

  void RemoveEventListener(EventListener eventListener) {
    mEventListeners.remove(eventListener);
  }
};
