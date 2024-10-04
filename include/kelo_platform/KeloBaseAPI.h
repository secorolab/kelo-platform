/**
 * @author Vamsi Kalagaturu (vamsikalagaturu@gmail.com)
 * @brief
 */

#ifndef KELO_BASE_API_H
#define KELO_BASE_API_H

#define MOTOR_CONST 0.29

typedef struct
{
  int nWheels;
  int *index_to_EtherCAT;
  double radius;
  double castor_offset;
  double half_wheel_distance;
  double *wheel_coordinates;
  double *pivot_angles_deviation;
} KeloBaseConfig;

#endif  // KELO_BASE_API_H