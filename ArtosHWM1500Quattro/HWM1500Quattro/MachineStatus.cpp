#include "MachineStatus.h"

MachineStatus::MachineStatus()
{
  MachineStatus::init();
}

// ------------------
// Public methods

//boolean _temp = false; // TODO remove this
void MachineStatus::checkOperationTime()
{
  //if (!_temp)
  //{
    //MachineStatus::updateOperationTime();
    //_temp = true;

    //DMSG(machineStatus.operationTime);
    //DMSG(machineStatus.serviceTime);
    //DMSG(machineStatus.quarters);
  //}
}

// ------------------
// Private methods

void MachineStatus::init()
{
  for (unsigned int t=0; t<sizeof(machineStatus); t++) {
    *((char*)&machineStatus + t) = EEPROM.read(t);
  }
}

void MachineStatus::updateOperationTime()
{
  //machineStatus.operationTime = 5;
  //machineStatus.serviceTime = 0;
  //machineStatus.quarters = 2;
  
  for (unsigned int t=0; t<sizeof(machineStatus); t++) {
    EEPROM.write(t, *((char*)&machineStatus + t));
  }
}
