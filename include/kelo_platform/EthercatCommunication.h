/**
 * @author Vamsi Kalagaturu (vamsikalagaturu@gmail.com)
 * @brief
 */

#ifndef KELO_MOTION_CONTROL_ETHERCAT_COMMUNICATION_H
#define KELO_MOTION_CONTROL_ETHERCAT_COMMUNICATION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "soem/ethercat.h"
#include "soem/ethercatbase.h"
#include "soem/ethercatcoe.h"
#include "soem/ethercatconfig.h"
#include "soem/ethercatdc.h"
#include "soem/ethercatmain.h"
#include "soem/ethercatprint.h"
#include "soem/ethercattype.h"
#include "soem/nicdrv.h"

#define IO_MAP_SIZE 4096

  typedef struct
  {
    ec_slavet ecx_slave[EC_MAXSLAVE];
    int ecx_slavecount;
    ec_groupt ec_group[EC_MAXGROUP];
    uint8 esibuf[EC_MAXEEPBUF];
    uint32 esimap[EC_MAXEEPBITMAP];
    ec_eringt ec_elist;
    ec_idxstackT ec_idxstack;
    ec_SMcommtypet ec_SMcommtype;
    ec_PDOassignt ec_PDOassign;
    ec_PDOdesct ec_PDOdesc;
    ec_eepromSMt ec_SM;
    ec_eepromFMMUt ec_FMMU;
    boolean EcatError;
    int64 ec_DCtime;
    ecx_portt ecx_port;
    ecx_redportt ecx_redport;
    ecx_contextt ecx_context;
    char IOmap[IO_MAP_SIZE];
  } EthercatConfig;

  void init_ecx_context(EthercatConfig *config);

  void establish_connection(EthercatConfig *config, char *ifname, int *result);

  void check_slave_state(EthercatConfig *config, uint16 required_state, int *result);

  void ethercat_connect(EthercatConfig *ethercat_config, char *ifname, int *result);

  void process_data_exchange(EthercatConfig *config);

  void send_and_receive_data(EthercatConfig *config);

#ifdef __cplusplus
}
#endif

#endif  // KELO_MOTION_CONTROL_ETHERCAT_COMMUNICATION_H