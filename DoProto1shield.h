
#ifndef _h_doproto1shield
#define _h_doproto1shield

#define DEVICEID     0x1000
#define ENDOFMESSAGE '~'

#define LEDRED       2
#define LEDYELLOW    3
#define LEDGREEN     4

#define MTYPEDOMOTICA  1
#define MTYPEDOTTIE    2

// Domotica devices for 'sEnseid'

#define IDLAMPONE        0x10000
#define IDLAMPTWO        0x10001
#define IDLAMPTHREE      0x10002
#define IDSWITCHONE      0x20000
#define IDSWITCHTWO      0x20001

// Domotica groups for 'group'

#define IDGRPONE       0x01

// Domotica instructions for 'instruction'

#define FETCHVALUE     0
#define SWITCHOFF      1
#define SWITCHON       2
#define DIMMER         3
#define BRIGHTER       4

typedef struct _dmsg1 { // message definition for type == 1
  long   sEnseid;
  int   instruction;
  int   group; 
  int   value;         // current value of device returned through FETCHVALUE instruction 
} DMSG1;

typedef struct _dmsg2 { // message definition for type == 1
  long   sEnseid;
  int   instruction;
  int   value;         // current value of device returned through FETCHVALUE instruction 
} DMSG2;

typedef struct _acdomessage {
  int   cOntrol; // allow ASCII for input message on monitor or straight binary depending on this value
  int    tYpe;
  int    lEn;     // size of internal message
  union  {
    char  cbUf[128];  // pointer to internal message
    int   ibUf[64];
    DMSG1 domotica;
    DMSG2 dottie;
  } mSg;
} ACDoMessage;


extern int ACDogetMessage(ACDoMessage *DoMsg);
extern int ACDoputMessage(ACDoMessage *DoMsg);

#endif
