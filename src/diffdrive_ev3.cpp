#include "diffdrive_ev3/diffdrive_ev3.h"


#include "hardware_interface/types/hardware_interface_type_values.hpp"




DiffDriveEv3::DiffDriveEv3()
    : logger_(rclcpp::get_logger("DiffDriveEv3"))
{}





CallbackReturn DiffDriveEv3::on_init(const hardware_interface::HardwareInfo & info)
{
  if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS)
  {
    return CallbackReturn::ERROR;
  }

  RCLCPP_INFO(logger_, "Configuring...");

  time_ = std::chrono::system_clock::now();

  cfg_.left_wheel_name = info_.hardware_parameters["left_wheel_name"];
  cfg_.right_wheel_name = info_.hardware_parameters["right_wheel_name"];
  cfg_.loop_rate = std::stof(info_.hardware_parameters["loop_rate"]);
  cfg_.ip = info_.hardware_parameters["ip"];
  cfg_.enc_counts_per_rev = std::stoi(info_.hardware_parameters["enc_counts_per_rev"]);

  // Set up the wheels
  l_wheel_.setup(cfg_.left_wheel_name, cfg_.enc_counts_per_rev);
  r_wheel_.setup(cfg_.right_wheel_name, cfg_.enc_counts_per_rev);

  ev3=new Ev3(cfg_.ip.c_str());

  RCLCPP_INFO(logger_, "Finished Configuration");

  return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface> DiffDriveEv3::export_state_interfaces()
{
  // We need to set up a position and a velocity interface for each wheel

  std::vector<hardware_interface::StateInterface> state_interfaces;

  state_interfaces.emplace_back(hardware_interface::StateInterface(l_wheel_.name, hardware_interface::HW_IF_VELOCITY, &l_wheel_.vel));
  state_interfaces.emplace_back(hardware_interface::StateInterface(l_wheel_.name, hardware_interface::HW_IF_POSITION, &l_wheel_.pos));
  state_interfaces.emplace_back(hardware_interface::StateInterface(r_wheel_.name, hardware_interface::HW_IF_VELOCITY, &r_wheel_.vel));
  state_interfaces.emplace_back(hardware_interface::StateInterface(r_wheel_.name, hardware_interface::HW_IF_POSITION, &r_wheel_.pos));

  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> DiffDriveEv3::export_command_interfaces()
{
  // We need to set up a velocity command interface for each wheel

  std::vector<hardware_interface::CommandInterface> command_interfaces;

  command_interfaces.emplace_back(hardware_interface::CommandInterface(l_wheel_.name, hardware_interface::HW_IF_VELOCITY, &l_wheel_.cmd));
  command_interfaces.emplace_back(hardware_interface::CommandInterface(r_wheel_.name, hardware_interface::HW_IF_VELOCITY, &r_wheel_.cmd));

  return command_interfaces;
}


CallbackReturn DiffDriveEv3::on_activate(const rclcpp_lifecycle::State & previous_state) 
{
  RCLCPP_INFO(logger_, "Starting Controller...");
 
  return CallbackReturn::SUCCESS;
}

CallbackReturn DiffDriveEv3::on_deactivate(const rclcpp_lifecycle::State & previous_state)
{
  RCLCPP_INFO(logger_, "Stopping Controller...");

  return CallbackReturn::SUCCESS;
}

hardware_interface::return_type DiffDriveEv3::read(const rclcpp::Time & time, const rclcpp::Duration & period)
{

  // TODO fix chrono duration

  // Calculate time delta
  auto new_time = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = new_time - time_;
  double deltaSeconds = diff.count();
  time_ = new_time;
  l_wheel_.enc=ev3->Gpos();
  r_wheel_.enc=ev3->Dpos();

  double pos_prev = l_wheel_.pos;
  l_wheel_.pos = l_wheel_.calcEncAngle();
  l_wheel_.vel = (l_wheel_.pos - pos_prev) / deltaSeconds;

  pos_prev = r_wheel_.pos;
  r_wheel_.pos = r_wheel_.calcEncAngle();
  r_wheel_.vel = (r_wheel_.pos - pos_prev) / deltaSeconds;



  return return_type::OK;

  
}

hardware_interface::return_type DiffDriveEv3::write(const rclcpp::Time & time, const rclcpp::Duration & period)
{
  ev3->Grun(l_wheel_.cmd);
  ev3->Drun(r_wheel_.cmd);
  
  return return_type::OK;
  
}



#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(
  DiffDriveEv3,
  hardware_interface::SystemInterface
)