/**
 * @author Vamsi Kalagaturu (vamsikalagaturu@gmail.com)
 * @brief
 */

#ifndef KELO_MOTION_CONTROL_MEDIATOR_H
#define KELO_MOTION_CONTROL_MEDIATOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "kelo_platform/EthercatCommunication.h"
#include "kelo_platform/KeloDriveCommunication.h"
#include "kelo_platform/PowerManagementUnit.h"
#include "kelo_platform/KeloBaseAPI.h"

  // Initialization Functions
  void establish_kelo_base_connection(KeloBaseConfig* kelo_base_config,
                                      EthercatConfig* ethercat_config, char* ifname,
                                      uint32 pmu_command, int* result);

  void get_kelo_base_state(KeloBaseConfig* kelo_base_config, EthercatConfig* ethercat_config,
                           double* pivot_angles, double* pivot_velocities,
                           double* wheel_encoder_values, double* wheel_angular_velocities);

  void get_kelo_wheel_voltages_and_currents(KeloBaseConfig* kelo_base_config,
                                            EthercatConfig* ethercat_config, double* bus_voltages,
                                            double* wheel_voltages, double* wheel_currents);

  void set_kelo_base_torques(KeloBaseConfig* kelo_base_config, EthercatConfig* ethercat_config,
                             double* wheel_torques);

  void update_base_state(KeloBaseConfig* kelo_base_config, EthercatConfig* ethercat_config);

#ifdef __cplusplus
}
#endif

#endif  // KELO_MOTION_CONTROL_MEDIATOR_H