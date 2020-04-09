#ifndef MDNS_HPP
#define MDNS_HPP

#include <mdns.h>
#include <string>

class MDNSManager {
public:
  MDNSManager() {};
  MDNSManager(std::string acc_name, std::string acc_model);

  void publish_record();

private:
  void update_state();

  std::string _acc_name;
  std::string _acc_model;
  std::string _status_flag;
  std::string _device_id;

  mdns_txt_item_t _hap_txt_data[7];
};


#endif // MDNS_HPP
