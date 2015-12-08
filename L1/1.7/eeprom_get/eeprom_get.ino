/***
    eeprom_get example.

    This shows how to use the EEPROM.get() method.

    To pre-set the EEPROM data, run the example sketch eeprom_put.
    This sketch will run without it, however, the values shown
    will be shown from what ever is already on the EEPROM.

    This may cause the serial object to print out a large string
    of garbage if there is no null character inside one of the strings
    loaded.

    Written by Christopher Andrews 2015
    Released under MIT licence.
***/

#include <EEPROM.h>

struct Data {
  uint8_t succ;
  uint8_t loss;
  uint32_t roundTrip;
  uint8_t rssi;
};

void setup() {

  int eeAddress = 0; //EEPROM address to start reading from
  Data data;
  byte value;

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Start reading from eeprom");
  for(int i = 0; i < EEPROM.length(); i++)
  {
    value = EEPROM.read(eeAddress+(i*sizeof(Data)));
    if(value != 'A')
    {
      //Get the float data from the EEPROM at position 'eeAddress'
      EEPROM.get(eeAddress+(i*sizeof(Data)), data);
      Serial.println("###############");
      Serial.println("RoundTrip: " + String(data.roundTrip) + " microseconds");
      Serial.println("Rssi: -" + String(data.rssi));

      Serial.println("PackageErrorRate: " + String((data.loss*1.0)/(data.loss+data.succ)) + " (" + data.loss + "/" + (data.loss+data.succ) + ")");
    }
    else
      break;
  }
  Serial.println("Done reading from EEPROM");
}

void loop() {
  /* Empty loop */
}
