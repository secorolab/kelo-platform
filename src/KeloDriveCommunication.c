#include "kelo_platform/KeloDriveCommunication.h"

void create_empty_rx_msg(rxpdo1_t *msg)
{
  memset(msg, 0, sizeof(rxpdo1_t));
  msg->timestamp = 1;
}

void create_rx_msg(rxpdo1_t *msg)
{
  msg->timestamp = time(NULL);
  msg->command1 = COM1_ENABLE1 | COM1_ENABLE2 | COM1_MODE_TORQUE;
  msg->limit1_p = 3;     // upper limit for first wheel
  msg->limit1_n = -3;    // lower limit for first wheel
  msg->limit2_p = 3;     // upper limit for second wheel
  msg->limit2_n = -3;    // lower limit for second wheel
  msg->setpoint1 = 0.0;  // setpoint for first wheel
  msg->setpoint2 = 0.0;  // setpoint for second wheel
}

void set_wheel_torques(EthercatConfig *config, rxpdo1_t *msg, int *index_to_EtherCAT,
                       double *wheel_torques, int nWheels, double motor_const)
{
  for (unsigned int i = 0; i < nWheels; i++)
  {
    // Update torque values in the message for each wheel
    // msg->setpoint1 = -motor_const * wheel_torques[2 * i];     // units: (rad/sec) for first
    // wheel msg->setpoint2 = motor_const * wheel_torques[2 * i + 1];  // units: (rad/sec) for
    // second wheel

    // units: (rad/sec)
    msg->setpoint1 = -wheel_torques[2 * i] / motor_const;  // negative due to inverted frame
    msg->setpoint2 = wheel_torques[2 * i + 1] / motor_const;

    // Get the output pointer for the current wheel based on EtherCAT mapping
    rxpdo1_t *ecData = (rxpdo1_t *)config->ecx_slave[index_to_EtherCAT[i]].outputs;

    // Copy the updated message to the slave's output
    memcpy(ecData, msg, sizeof(rxpdo1_t));
  }
}

void read_encoder_values(EthercatConfig *config, double *pivot_angles, double *pivot_velocities,
                         int *index_to_EtherCAT, int nWheels, double *pivot_angles_deviation,
                         double *wheel_encoder_values, double *wheel_angular_velocities)
{
  for (unsigned int i = 0; i < nWheels; i++)
  {
    txpdo1_t *ecData = (txpdo1_t *)config->ecx_slave[index_to_EtherCAT[i]].inputs;
    pivot_angles[i] = ecData->encoder_pivot - pivot_angles_deviation[i];
    if (pivot_angles[i] > 2 * M_PI)
      pivot_angles[i] -= 2 * M_PI;
    else if (pivot_angles[i] < 0.0)
      pivot_angles[i] += 2 * M_PI;

    pivot_velocities[i] = ecData->velocity_pivot;

    wheel_encoder_values[2 * i] = ecData->encoder_1;
    wheel_encoder_values[2 * i + 1] = ecData->encoder_2;

    wheel_angular_velocities[2 * i] = ecData->velocity_1;
    wheel_angular_velocities[2 * i + 1] = ecData->velocity_2;
  }
}

void read_voltages_and_currents(EthercatConfig *config, int *index_to_EtherCAT, int nWheels,
                                double *bus_voltages, double *wheel_voltages,
                                double *wheel_currents)
{
  for (unsigned int i = 0; i < nWheels; i++)
  {
    txpdo1_t *ecData = (txpdo1_t *)config->ecx_slave[index_to_EtherCAT[i]].inputs;
    wheel_voltages[2 * i] = ecData->voltage_1;
    wheel_voltages[2 * i + 1] = ecData->voltage_2;

    bus_voltages[i] = ecData->voltage_bus;

    double u1 = ecData->current_1_u;
    double v1 = ecData->current_1_v;
    double w1 = ecData->current_1_w;
    wheel_currents[2 * i] = sqrt((u1 * u1 + v1 * v1 + w1 * w1) / 3.0);

    double u2 = ecData->current_2_u;
    double v2 = ecData->current_2_v;
    double w2 = ecData->current_2_w;
    wheel_currents[2 * i + 1] = sqrt((u2 * u2 + v2 * v2 + w2 * w2) / 3.0);
  }
}