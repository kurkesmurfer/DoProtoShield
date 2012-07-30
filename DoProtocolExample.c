

#include <DoProto1shield.h>

ACDoMessage myDoMsg;

main() {
  int TobeReportedToSense;
  
  /* Message to request value of sensor
     Requires id of sensor to allow 433 MHz Layer to
     identify the device

     Future: Maybe a separate namespace to unify id's?

  */

  myDoMsg.cOntrol = 'b';
  myDoMsg.tYpe = 1;
  myDoMsg.lEn = 1;
  myDoMsg.mSg.domotica.sEnseid = IDLAMPONE;
  myDoMsg.mSg.domotica.instruction = FETCHVALUE;
  myDoMsg.mSg.domotica.group = IDGRPONE;
  

  
  ACDoputMessage(&myDoMsg); // writes request to serial port
  
  if (ACHDogetMessage(&myDoMsg)) // reads reply from serial port
	ACDoprintMessage(&myDoMsg);
  
  TobeReportedToSense = myDoMsg.mSg.domotica.value;
  
    
}
