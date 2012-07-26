

#include "DoProto1shield.h"


const int buttonPin = 6;

int buttonState = 0;
int rood,geel,groen;
unsigned int KlikKlak[2];
int lamps[10];
int lampvalues[10];
int switches[10];
int switchvalues[10];

ACDoMessage myDoMsg;

int ACDogetMessage(ACDoMessage *DoMsg) {
  int incoming;
  int index=0;
  char localbuf[128];
  
  incoming = Serial.read();
  if (incoming == -1) return(0);
  
  DoMsg->cOntrol=incoming;
  switch (DoMsg->cOntrol) {
    case 'A': 
    case 'a': // ASCII
          Serial.print(DoMsg->cOntrol);
          while (incoming != ENDOFMESSAGE) {
            incoming = Serial.read();
            if (incoming != -1)
              Serial.print(incoming);
            if (incoming & 0x30 && incoming < 58) {
              DoMsg->tYpe = incoming & 0xF;
              if (DoMsg->tYpe == 1) {
                DoMsg->mSg.domotica.sEnseid = 0x10000;
                Serial.print(DoMsg->mSg.domotica.instruction = Serial.read() & 0xF);
                DoMsg->mSg.domotica.group = IDGRPONE;
                incoming = '~'; // end of message
                index = sizeof(DMSG1);           
              } else if (DoMsg->tYpe == 2) {
                DoMsg->mSg.dottie.sEnseid = 0x10000; // not valid for dottie
                Serial.print(DoMsg->mSg.dottie.instruction = Serial.read() & 0xF);
                incoming = '~'; // end of message 
                index = sizeof(DMSG2);           
              } else {
                incoming = Serial.read();
                if (incoming != -1)
                  Serial.print(DoMsg->mSg.cbUf[index++] = (char)incoming);
              }
            }
          }
          break;
    case 'B': // Binary
    case 'b':
          DoMsg->tYpe = Serial.read();
          DoMsg->lEn  = Serial.read();
          while (index < DoMsg->lEn)
            DoMsg->mSg.cbUf[index++] = Serial.read();
          break;
  }

  return(index);
};

int ACDoputMessage(ACDoMessage *DoMsg) {
  int i;  
  Serial.write(DoMsg->cOntrol);
  Serial.write(DoMsg->tYpe);
  Serial.write(DoMsg->lEn);

  for (i=0;i<DoMsg->lEn;i++)
    Serial.write(DoMsg->mSg.cbUf[i]);
  return(0);
};


int ACDoprintMessage(ACDoMessage *DoMsg) {
  int i;

  Serial.println();  
  Serial.print("cOntrol: ");
  Serial.write(DoMsg->cOntrol);
  Serial.print(" tYpe: ");
  Serial.print(DoMsg->tYpe,DEC);
  Serial.print(" lEn: ");
  Serial.print(DoMsg->lEn,DEC);
  Serial.println("'Content:");
  if (DoMsg->tYpe == 1) {
    Serial.print("sEnseid :");
    Serial.println(DoMsg->mSg.domotica.sEnseid);
    Serial.print("instruction :");
    Serial.println(DoMsg->mSg.domotica.instruction);
    Serial.print("group :");
    Serial.println(DoMsg->mSg.domotica.group);
    Serial.print("value :");
    Serial.println(DoMsg->mSg.domotica.value);
  }
  else  if (DoMsg->tYpe == 2) {
    Serial.print("sEnseid :");
    Serial.println(DoMsg->mSg.dottie.sEnseid);
    Serial.print("instruction :");
    Serial.println(DoMsg->mSg.dottie.instruction);
    Serial.print("group :");
    Serial.println(DoMsg->mSg.dottie.value);
  } else {  
  for (i=0;i<DoMsg->lEn;i++)
    Serial.print(DoMsg->mSg.cbUf[i],HEX); // aal undefined msg types
  Serial.println();
  }
  return(0);
};
          
void ACDoLamps(DMSG1 *domo,int index) {
    if (domo->instruction == FETCHVALUE)
          domo->value = lampvalues[index];          
    return;
}

void ACDoSwitches(DMSG1 *domo,int index) {
    if (domo->instruction == FETCHVALUE)
          domo->value = switchvalues[index];          
    return;
}

void ACHandleRequest(ACDoMessage *DoMsg) {
int i;
  switch (DoMsg->tYpe) {
    case 1: // handle type 1 Domotica
          for (i=0;i<10;i++) {
            if (DoMsg->mSg.domotica.sEnseid == lamps[i]) {
                ACDoLamps(&(DoMsg->mSg.domotica),i);
                break;
            }
            if (DoMsg->mSg.domotica.sEnseid == switches[i]) {
                ACDoSwitches(&(DoMsg->mSg.domotica),i);
                break;
            }
          }
          break;
    case 2: // handle type 2 Dottie
          //DoMsg->mSg.dottie.sEnseid;
         // DoMsg->mSg.dottie.instruction;
          break;
    }
  if (DoMsg->cOntrol == 'a' || DoMsg->cOntrol == 'A')
      ACDoprintMessage(DoMsg);
  
  else if (DoMsg->cOntrol == 'b' || DoMsg->cOntrol == 'B')
      ACDoputMessage(DoMsg);

}

void setup()
{
  int i;
  
  pinMode(LEDRED, OUTPUT);     // leds dobot one
  pinMode(LEDYELLOW, OUTPUT);     
  pinMode(LEDGREEN, OUTPUT);     

  pinMode(buttonPin, INPUT);

  rood = 0;
  geel = 0;
  groen = 0;
  
  digitalWrite(LEDRED,rood);
  digitalWrite(LEDYELLOW,geel);
  digitalWrite(LEDGREEN,groen);
  
  Serial.begin(115200); // standard monitor



  KlikKlak[0] = DEVICEID;
  KlikKlak[1] = 0;

  for (i=0;i<10;i++) {
    lamps[i]    = IDLAMPONE | i;
    switches[i] = IDSWITCHONE | i;
  }
  
  myDoMsg.cOntrol = 'b';
  myDoMsg.tYpe = 1;
  myDoMsg.lEn = sizeof(DMSG1);
  myDoMsg.mSg.domotica.sEnseid = IDLAMPONE;
  myDoMsg.mSg.domotica.instruction = FETCHVALUE;
  myDoMsg.mSg.domotica.group = IDGRPONE;

  ACDoprintMessage(&myDoMsg);

  Serial.println("Ready ->");

}

void loop()
{
  int i;
  
  if (ACDogetMessage(&myDoMsg)) {
      if (myDoMsg.cOntrol == 'a' || myDoMsg.cOntrol == 'A') 
        ACDoprintMessage(&myDoMsg);    // debugging the message interface
//      else
//        ACDoputMessage(&myDoMsg);
  
      //ACHandleRequest(&myDoMsg);
  }
  
}
