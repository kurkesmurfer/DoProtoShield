

#include <DoProto1shield.h>

ACDoMessage myDoMsg;

main() {
  int TobeReportedToSense;
  

  myDoMsg.cOntrol = 'b';
  myDoMsg.tYpe = 1;
  myDoMsg.lEn = 1;
  myDoMsg.mSg.domotica.sEnseid = IDLAMPONE;
  myDoMsg.mSg.domotica.instruction = FETCHVALUE;
  myDoMsg.mSg.domotica.group = IDGRPONE;
  

  
  ACDoputMessage(&myDoMsg); // writes request to serial port
  
  ACHDogetMessage(&myDoMsg); // reads reply from serial port
  
  TobeReportedToSense = myDoMsg.mSg.domotica.value;
  
    
}
