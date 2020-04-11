#include "event_manager.hpp"

#include <algorithm>
#include <esp_log.h>

#include "json.hpp"

using namespace std;

void EventManager::session_closed(void* session_id) {
  for(auto it = _event_subs.begin(); it != _event_subs.end(); it++) {
    if(it->first->get_identifier() == session_id) {
      _event_subs.erase(it);
      break;
    }
  }
}

void EventManager::register_for_events(ISession* session, uint64_t char_id) {
  _event_subs[session].push_back(char_id);
}

void EventManager::unregister_for_events(ISession* session, uint64_t char_id) {
  auto& session_subs = _event_subs[session];
  for(auto it = session_subs.begin(); it != session_subs.end(); it++) {
    if(*it == char_id) {
      session_subs.erase(it);
      break;
    }
  }
  if(session_subs.size() < 1) { _event_subs.erase(session); }
}

void EventManager::characteristic_updated(uint64_t acc_id, uint64_t char_id) {
  _queued_events.push_back({acc_id, char_id});
}

void EventManager::flush() {
  if(_queued_events.size() < 1) { return; }

  // Publish to subscribers
  for(auto& sub : _event_subs) {
    nlohmann::json event_data;

    for(auto& event : _queued_events) {
      if(find(sub.second.begin(), sub.second.end(), event.second) != sub.second.end()) {
        auto characteristic = _acc_db.find_char(event.first, event.second);
        if(!characteristic) { continue; }
        event_data["characteristics"].push_back({
          {"aid", event.first},
          {"iid", event.second},
          {"value", characteristic->handle_read()}
        });
      }
    }

    auto body = event_data.dump();
    ESP_LOGD("ev-mgr", "Sending event to controller: %s", body.c_str());
    sub.first->event(body);
  }

  _queued_events.clear();
}
