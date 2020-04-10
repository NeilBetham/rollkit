#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <cstdint>
#include <list>
#include <utility>

#include "i_session.hpp"
#include "i_session_event_delegate.hpp"
#include "accessory_db.hpp"

class EventManager : public ISessionEventDelegate {
public:
  EventManager(AccessoryDB& acc_db) : _acc_db(acc_db) {};

  void session_closed(void* session_id);

  void register_for_events(ISession* session, uint64_t char_id);
  void unregister_for_events(ISession* session, uint64_t char_id);
  void characteristic_updated(uint64_t acc_id, uint64_t char_id);

private:
  AccessoryDB& _acc_db;
  std::list<std::pair<ISession*, uint64_t>> _event_subs;
};

#endif // EVENT_MANAGER_HPP
