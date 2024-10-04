/**
 * @file EthercatCommunication.c
 * @author Kavya Shankar (kavya.shankar@smail.inf.h-brs.de)
 * @brief Establishing connection with ehtercat and performing data transfer with robot
 * @date 2022-03-12
 *
 */
#include "kelo_platform/EthercatCommunication.h"

void init_ecx_context(EthercatConfig *config)
{
  // Initialize context pointers to the relevant parts of the configuration structure.
  config->ecx_context.port = &config->ecx_port;
  config->ecx_context.slavelist = &config->ecx_slave[0];
  config->ecx_context.slavecount = &config->ecx_slavecount;
  config->ecx_context.maxslave = EC_MAXSLAVE;
  config->ecx_context.grouplist = &config->ec_group[0];
  config->ecx_context.maxgroup = EC_MAXGROUP;
  config->ecx_context.esibuf = &config->esibuf[0];
  config->ecx_context.esimap = &config->esimap[0];
  config->ecx_context.esislave = 0;  // Assuming initial value as 0
  config->ecx_context.elist = &config->ec_elist;
  config->ecx_context.idxstack = &config->ec_idxstack;

  config->ecx_context.ecaterror = &config->EcatError;
  config->ecx_context.DCtime = &config->ec_DCtime;
  config->ecx_context.SMcommtype = &config->ec_SMcommtype;
  config->ecx_context.PDOassign = &config->ec_PDOassign;
  config->ecx_context.PDOdesc = &config->ec_PDOdesc;
  config->ecx_context.eepSM = &config->ec_SM;
  config->ecx_context.eepFMMU = &config->ec_FMMU;
  config->ecx_context.manualstatechange = 0;  // 0 typically means no manual state change allowed
}

void establish_connection(EthercatConfig *config, char *ifname, int *result)
{
  if (!ecx_init(&config->ecx_context, ifname))
  {
    printf("Failed to initialize EtherCAT\n");
    *result = -1;
  }
  int wkc = ecx_config_init(&config->ecx_context, TRUE);
  if (wkc <= 0)
  {
    printf("No slaves found\n");
    *result = -1;
  }
  else
  {
    printf("Found %d slaves\n", wkc);
    *result = 0;
  }
  int iomap_size = ecx_config_map_group(&config->ecx_context, config->IOmap, 0);
  printf("IOmap size: %d\n", iomap_size);
  *result = 0;
}

void check_slave_state(EthercatConfig *config, uint16 required_state, int *result)
{
  ecx_statecheck(&config->ecx_context, 0, required_state, EC_TIMEOUTSTATE);

  if (config->ecx_slave[0].state != required_state)
  {
    ecx_readstate(&config->ecx_context);
    for (int i = 1; i <= config->ecx_slavecount; i++)
    {
      if (config->ecx_slave[i].state != required_state)
      {
        printf("Slave %i State= %i\n", i, config->ecx_slave[i].state);
      }
    }
    *result = -1;
  }
  *result = 0;
}

void ethercat_connect(EthercatConfig *ethercat_config, char *ifname, int *result)
{
  *result = 0;

  establish_connection(ethercat_config, ifname, result);
  if (*result == -1)
  {
    printf("Failed to establish connection\n");
    return;
  }

  check_slave_state(ethercat_config, EC_STATE_SAFE_OP, result);
  if (*result == -1)
  {
    printf("EtherCAT slaves have not reached safe operational state\n");
    return;
  }
}

void process_data_exchange(EthercatConfig *config)
{
  ecx_send_processdata(&config->ecx_context);
  ecx_receive_processdata(&config->ecx_context, EC_TIMEOUTRET);
  config->ecx_slave[0].state = EC_STATE_OPERATIONAL;
  // ecx_send_processdata(&config->ecx_context);
  ecx_writestate(&config->ecx_context, 0);
}

void send_and_receive_data(EthercatConfig *config)
{
  ecx_send_processdata(&config->ecx_context);
  ecx_receive_processdata(&config->ecx_context, EC_TIMEOUTRET);
}
