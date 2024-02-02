#ifndef EV3_CONFIG_H
#define EV3__CONFIG_H

#include <string>


struct Config
{
  std::string left_wheel_name = "front_wheel_left_joint";
  std::string right_wheel_name = "front_wheel_right_joint";
  float loop_rate = 30;
  std::string ip;
  int enc_counts_per_rev = 360;
};


#endif