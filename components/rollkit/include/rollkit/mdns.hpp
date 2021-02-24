#ifndef MDNS_HPP
#define MDNS_HPP

#include <mdns.h>
#include <string>

namespace rollkit {


class MDNSManager {
public:
  MDNSManager() {};
  MDNSManager(std::string acc_name, std::string acc_model, std::string mac);

  void publish_record();

private:
  void update_state();

  std::string _acc_name;
  std::string _acc_model;
  std::string _status_flag;
  std::string _device_id;

  mdns_txt_item_t _hap_txt_data[7];
};


} // namespace rollkit
#endif // MDNS_HPP
