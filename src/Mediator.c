#include "kelo_platform/Mediator.h"

void establish_kelo_base_connection(KeloBaseConfig* kelo_base_config,
                                    EthercatConfig* ethercat_config, char* ifname,
                                    uint32 pmu_command, int* result)
{
  *result = 0;

  ethercat_connect(ethercat_config, ifname, result);

  // set the PMU command
  command_pmu(ethercat_config, pmu_command);
  process_data_exchange(ethercat_config);

  rxpdo1_t msg;
  create_empty_rx_msg(&msg);

  for (size_t i = 0; i < kelo_base_config->nWheels; i++)
  {
    rxpdo1_t* ecData =
        (rxpdo1_t*)ethercat_config->ecx_slave[kelo_base_config->index_to_EtherCAT[i]].outputs;
    memcpy(ecData, &msg, sizeof(msg));
  }

  process_data_exchange(ethercat_config);
  check_slave_state(ethercat_config, EC_STATE_OPERATIONAL, result);
  if (*result == -1)
  {
    printf("EtherCAT slaves have not reached operational state\n");
    return;
  }

  printf("Connection established!\n\n");
}

void update_base_state(KeloBaseConfig* kelo_base_config, EthercatConfig* ethercat_config)
{
  send_and_receive_data(ethercat_config);
}

void get_kelo_base_state(KeloBaseConfig* kelo_base_config, EthercatConfig* ethercat_config,
                         double* pivot_angles, double* pivot_velocities,
                         double* wheel_encoder_values, double* wheel_angular_velocities)
{
  read_encoder_values(ethercat_config, pivot_angles, pivot_velocities,
                      kelo_base_config->index_to_EtherCAT, kelo_base_config->nWheels,
                      kelo_base_config->pivot_angles_deviation, wheel_encoder_values,
                      wheel_angular_velocities);
}

void get_kelo_wheel_voltages_and_currents(KeloBaseConfig* kelo_base_config,
                                          EthercatConfig* ethercat_config, double* bus_voltages,
                                          double* wheel_voltages, double* wheel_currents)
{
  read_voltages_and_currents(ethercat_config, kelo_base_config->index_to_EtherCAT,
                             kelo_base_config->nWheels, bus_voltages, wheel_voltages,
                             wheel_currents);
}

void set_kelo_base_torques(KeloBaseConfig* kelo_base_config, EthercatConfig* ethercat_config,
                           double* wheel_torques)
{
  rxpdo1_t rx_msg;
  memset(&rx_msg, 0, sizeof(rx_msg));
  create_rx_msg(&rx_msg);
  set_wheel_torques(ethercat_config, &rx_msg, kelo_base_config->index_to_EtherCAT, wheel_torques,
                    kelo_base_config->nWheels, MOTOR_CONST);
}