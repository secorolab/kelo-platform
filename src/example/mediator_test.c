#include "kelo_platform/Mediator.h"
#include <stdlib.h>

int main()
{
  KeloBaseConfig *kelo_base_config = calloc(1, sizeof(*kelo_base_config));
  kelo_base_config->nWheels = 4;
  kelo_base_config->index_to_EtherCAT = (int[4]){3, 4, 6, 7};
  kelo_base_config->radius = 0.115 / 2;
  kelo_base_config->castor_offset = 0.01;
  kelo_base_config->half_wheel_distance = 0.0775 / 2;
  kelo_base_config->wheel_coordinates = (double[8]){0.195, 0.21, -0.195, 0.21, -0.195, -0.21, 0.195, -0.21};
  kelo_base_config->pivot_angles_deviation = (double[4]){0.0, 0.0, 0.0, 0.0};
  
  EthercatConfig *ethercat_config = calloc(1, sizeof(*ethercat_config));
  init_ecx_context(ethercat_config);

  char ifname[] = "eno1";
  int result = 0;
  uint32 pmu_command = BASE_ON;

  establish_kelo_base_connection(kelo_base_config, ethercat_config, ifname, pmu_command, &result);

  double pivot_angles[4] = {0.0};
  double pivot_velocities[4] = {0.0};
  double wheel_torques[8] = {0.0};

  printf("Reading pivot angles\n");

  double wheel_encoder_values[8] = {0.0};
  double prev_wheel_encoders[8] = {0.0};
  double wheel_angular_velocities[8] = {0.0};

  get_kelo_base_state(kelo_base_config, ethercat_config, pivot_angles, pivot_velocities, wheel_encoder_values,
                      wheel_angular_velocities);

  for (size_t i = 0; i < 1; i++)
  {
    wheel_torques[2 * i]     = 1.0;
    wheel_torques[2 * i + 1] = 0.0;
  }

  int counter = 0;
  while (true)
  {
    printf("Counter: %d\n", counter);
    // usleep(10000);
    set_kelo_base_torques(kelo_base_config, ethercat_config, wheel_torques);
    send_and_receive_data(ethercat_config);
    get_kelo_base_state(kelo_base_config, ethercat_config, pivot_angles, pivot_velocities, wheel_encoder_values,
                        wheel_angular_velocities);
    printf("Pivot angles: ");
    for (size_t i = 0; i < 4; i++)
    {
      printf("%f ", pivot_angles[i]);
    }
    printf("\n");
    counter++;
  }

  free(kelo_base_config);
  free(ethercat_config);

  return 0;
}