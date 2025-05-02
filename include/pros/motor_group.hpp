

#ifndef _PROS_MOTOR_GROUP_HPP_
#define _PROS_MOTOR_GROUP_HPP_

#include <cstdint>
#include <iostream>

#include "pros/abstract_motor.hpp"
#include "pros/colors.hpp"
#include "pros/device.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "rtos.hpp"

namespace pros {
inline namespace v5 {
class MotorGroup : public virtual AbstractMotor {

public:
  MotorGroup(
      const std::initializer_list<std::int8_t>,
      const pros::v5::MotorGears gearset = pros::v5::MotorGears::invalid,
      const pros::v5::MotorUnits encoder_units = pros::v5::MotorUnits::invalid);

  MotorGroup(
      const std::vector<std::int8_t> &ports,
      const pros::v5::MotorGears gearset = pros::v5::MotorGears::invalid,
      const pros::v5::MotorUnits encoder_units = pros::v5::MotorUnits::invalid);

  MotorGroup(AbstractMotor &motor_group);

  std::int32_t move(std::int32_t voltage) const;

  std::int32_t move_absolute(const double position,
                             const std::int32_t velocity) const;

  std::int32_t move_relative(const double position,
                             const std::int32_t velocity) const;

  std::int32_t move_velocity(const std::int32_t velocity) const;

  std::int32_t move_voltage(const std::int32_t voltage) const;

  std::int32_t brake(void) const;

  std::int32_t modify_profiled_velocity(const std::int32_t velocity) const;

  double get_target_position(const std::uint8_t index) const;

  std::vector<double> get_target_position_all(void) const;

  std::int32_t get_target_velocity(const std::uint8_t index = 0) const;

  std::vector<std::int32_t> get_target_velocity_all(void) const;

  double get_actual_velocity(const std::uint8_t index = 0) const;

  std::vector<double> get_actual_velocity_all(void) const;

  std::int32_t get_current_draw(const std::uint8_t index = 0) const;

  std::vector<std::int32_t> get_current_draw_all(void) const;

  std::int32_t get_direction(const std::uint8_t index = 0) const;

  std::vector<std::int32_t> get_direction_all(void) const;

  double get_efficiency(const std::uint8_t index = 0) const;

  std::vector<double> get_efficiency_all(void) const;

  std::uint32_t get_faults(const std::uint8_t index = 0) const;

  std::vector<std::uint32_t> get_faults_all(void) const;

  std::uint32_t get_flags(const std::uint8_t index = 0) const;

  std::vector<std::uint32_t> get_flags_all(void) const;

  double get_position(const std::uint8_t index = 0) const;

  std::vector<double> get_position_all(void) const;

  double get_power(const std::uint8_t index = 0) const;

  std::vector<double> get_power_all(void) const;

  std::int32_t get_raw_position(std::uint32_t *const timestamp,
                                const std::uint8_t index = 0) const;

  std::vector<std::int32_t>
  get_raw_position_all(std::uint32_t *const timestamp) const;

  double get_temperature(const std::uint8_t index = 0) const;

  std::vector<double> get_temperature_all(void) const;

  double get_torque(const std::uint8_t index = 0) const;

  std::vector<double> get_torque_all(void) const;

  std::int32_t get_voltage(const std::uint8_t index = 0) const;

  std::vector<std::int32_t> get_voltage_all(void) const;

  std::int32_t is_over_current(const std::uint8_t index = 0) const;

  std::vector<std::int32_t> is_over_current_all(void) const;

  std::int32_t is_over_temp(const std::uint8_t index = 0) const;

  std::vector<std::int32_t> is_over_temp_all(void) const;

  MotorBrake get_brake_mode(const std::uint8_t index = 0) const;

  std::vector<MotorBrake> get_brake_mode_all(void) const;

  std::int32_t get_current_limit(const std::uint8_t index = 0) const;

  std::vector<std::int32_t> get_current_limit_all(void) const;

  MotorUnits get_encoder_units(const std::uint8_t index = 0) const;

  std::vector<MotorUnits> get_encoder_units_all(void) const;

  MotorGears get_gearing(const std::uint8_t index = 0) const;

  std::vector<MotorGears> get_gearing_all(void) const;

  std::vector<std::int8_t> get_port_all(void) const;

  std::int32_t get_voltage_limit(const std::uint8_t index = 0) const;

  std::vector<std::int32_t> get_voltage_limit_all(void) const;

  std::int32_t is_reversed(const std::uint8_t index = 0) const;

  std::vector<std::int32_t> is_reversed_all(void) const;

  MotorType get_type(const std::uint8_t index = 0) const;

  std::vector<MotorType> get_type_all(void) const;

  std::int32_t set_brake_mode(const MotorBrake mode,
                              const std::uint8_t index = 0) const;

  std::int32_t set_brake_mode(const pros::motor_brake_mode_e_t mode,
                              const std::uint8_t index = 0) const;

  std::int32_t set_brake_mode_all(const MotorBrake mode) const;

  std::int32_t set_brake_mode_all(const pros::motor_brake_mode_e_t mode) const;

  std::int32_t set_current_limit(const std::int32_t limit,
                                 const std::uint8_t index = 0) const;

  std::int32_t set_current_limit_all(const std::int32_t limit) const;

  std::int32_t set_encoder_units(const MotorUnits units,
                                 const std::uint8_t index = 0) const;

  std::int32_t set_encoder_units(const pros::motor_encoder_units_e_t units,
                                 const std::uint8_t index = 0) const;

  std::int32_t set_encoder_units_all(const MotorUnits units) const;

  std::int32_t
  set_encoder_units_all(const pros::motor_encoder_units_e_t units) const;

  std::int32_t set_gearing(std::vector<pros::motor_gearset_e_t> gearsets) const;

  std::int32_t set_gearing(const pros::motor_gearset_e_t gearset,
                           const std::uint8_t index = 0) const;

  std::int32_t set_gearing(std::vector<MotorGears> gearsets) const;

  std::int32_t set_gearing(const MotorGears gearset,
                           const std::uint8_t index = 0) const;

  std::int32_t set_gearing_all(const MotorGears gearset) const;

  std::int32_t set_gearing_all(const pros::motor_gearset_e_t gearset) const;

  std::int32_t set_reversed(const bool reverse, const std::uint8_t index = 0);

  std::int32_t set_reversed_all(const bool reverse);

  std::int32_t set_voltage_limit(const std::int32_t limit,
                                 const std::uint8_t index = 0) const;

  std::int32_t set_voltage_limit_all(const std::int32_t limit) const;

  std::int32_t set_zero_position(const double position,
                                 const std::uint8_t index = 0) const;

  std::int32_t set_zero_position_all(const double position) const;

  std::int32_t tare_position(const std::uint8_t index = 0) const;

  std::int32_t tare_position_all(void) const;

  std::int8_t size(void) const;

  std::int8_t get_port(const std::uint8_t index = 0) const;

  void operator+=(AbstractMotor &);

  void append(AbstractMotor &);

  void erase_port(std::int8_t port);

private:
  std::vector<std::int8_t> _ports;
  mutable pros::Mutex _MotorGroup_mutex;
};
} // namespace v5
} // namespace pros
#endif