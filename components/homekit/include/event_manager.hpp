#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <cstdint>
#include <unordered_map>
#include <list>
#include <utility>

#include "accessory_db.hpp"
#include "i_session.hpp"
#include "i_session_event_delegate.hpp"
#include "json.hpp"


class EventManager : public ISessionEventDelegate {
public:
  EventManager(AccessoryDB& acc_db) : _acc_db(acc_db) {};

  void session_closed(void* session_id);

  void register_for_events(ISession* session, uint64_t char_id);
  void unregister_for_events(ISession* session, uint64_t char_id);
  void characteristic_updated(uint64_t acc_id, uint64_t char_id);
  void flush();

private:
  AccessoryDB& _acc_db;
  std::unordered_map<ISession*, std::list<uint64_t>> _event_subs;
  std::list<std::pair<uint64_t, uint64_t>> _queued_events;
};

#endif // EVENT_MANAGER_HPP
