#ifndef EEPROMHelper_h
#define EEPROMHelper_h

#include "Arduino.h"
#include "EEPROM.h"

class EEPROMHelper
{
	public:
		//EEPROMHelper();
		//~EEPROMHelper();
		int ReadInt(int address);
        long ReadLong(int address);
		void WriteInt(int address, int value);
		void WriteLong(int address, long value);
		void WriteSingleValue(int address, int value);
};

#endif
