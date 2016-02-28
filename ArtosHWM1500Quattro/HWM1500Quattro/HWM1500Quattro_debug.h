#ifndef __DEBUG_H__
#define __DEBUG_H__

// uncomment this to print value on serial port
#define DEBUG

#include "Arduino.h"

#ifdef DEBUG
#define DMSG(args...)       Serial.println(args)
#define DMSG_STR(str)       Serial.println(str)
#define DMSG_HEX(num)       Serial.print(' '); Serial.print(num, HEX)
#define DMSG_INT(num)       Serial.print(' '); Serial.print(num)
#else
#define DMSG(args...)
#define DMSG_STR(str)
#define DMSG_HEX(num)
#define DMSG_INT(num)
#endif

#endif
