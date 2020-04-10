#include "event_manager.hpp"

#include <algorithm>

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
  _event_subs.push_back({session, char_id});
}

void EventManager::unregister_for_events(ISession* session, uint64_t char_id) {
  for(auto it = _event_subs.begin(); it != _event_subs.end(); it++) {
    if(it->first == session && it->second == char_id) {
      _event_subs.erase(it);
      break;
    }
  }
}


void EventManager::characteristic_updated(uint64_t acc_id, uint64_t char_id) {
  // Setup event data to send to subscribers
  auto characteristic = _acc_db.find_char(0, char_id);
  nlohmann::json event_data;
  event_data["characteristics"].push_back({
    {"aid", acc_id},
    {"iid", char_id},
    {"value", characteristic->handle_read()}
  });

  // Publish to subscribers
  for(auto& sub : _event_subs) {
    if(sub.second == char_id) {
      if(!characteristic) { break; }
      sub.first->event(event_data.dump());
    }
  }
}
