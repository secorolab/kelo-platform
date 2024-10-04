/**
 * @author Vamsi Kalagaturu (vamsikalagaturu@gmail.com)
 * @brief
 */

#ifndef KELO_DRIVE_COMMUNICATION_H
#define KELO_DRIVE_COMMUNICATION_H

#include "kelo_platform/EthercatCommunication.h"
#include "kelo_platform/KeloDriveAPI.h"
#include <math.h>
#define _USE_MATH_DEFINES

#ifdef __cplusplus
extern "C"
{
#endif

  void create_empty_rx_msg(rxpdo1_t *msg);

  void create_rx_msg(rxpdo1_t *msg);

  void set_wheel_torques(EthercatConfig *config, rxpdo1_t *msg, int *index_to_EtherCAT,
                         double *wheel_torques, int nWheels, double motor_const);

  void read_encoder_values(EthercatConfig *config, double *pivot_angles, double *pivot_velocities,
                           int *index_to_EtherCAT, int nWheels, double *pivot_angles_deviation,
                           double *wheel_encoder_values, double *wheel_angular_velocities);

  void read_voltages_and_currents(EthercatConfig *config, int *index_to_EtherCAT, int nWheels,
                                  double *bus_voltages, double *wheel_voltages,
                                  double *wheel_currents);

#ifdef __cplusplus
}
#endif

#endif  // KELO_DRIVE_COMMUNICATION_H