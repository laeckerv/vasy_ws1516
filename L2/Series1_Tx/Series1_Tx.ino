/**
   Copyright (c) 2009 Andrew Rapp. All rights reserved.

   This file is part of XBee-Arduino.

   XBee-Arduino is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   XBee-Arduino is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <XBee.h>
#include <EEPROM.h>

/*
  This example is for Series 1 XBee
  Sends a TX16 or TX64 request with the value of analogRead(pin5) and checks the status response for success
  Note: In my testing it took about 15 seconds for the XBee to start reporting success, so I've added a startup delay
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

unsigned long start = millis();

// Route commands
// 1 = Forward
// 2 = Backward
// 3 = Left
// 4 = Right
// 5 = Stop
// Anything else = Exit program on remote
const int n = 3;
uint8_t route[n][2] = {
  {'1', '0'},
  {'1', '3'},
  {'2', '3'}
};
uint8_t payload[] = {0};

// with Series 1 you can use either 16-bit or 64-bit addressing
// 16-bit addressing: Enter address of remote XBee, typically the coordinator
Tx16Request tx = Tx16Request(0xffff, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();
Rx16Response rx16 = Rx16Response();

int pin5 = 0;
int led = 13;
bool on = true;

unsigned long t_stamp_latency = 0;
unsigned long t_stamp_capacity = 0;
uint8_t succ = 0;
uint8_t loss = 0;
int eeAddress = 0;
int loopCnt = 0;

struct Data {
  uint8_t succ;
  uint8_t loss;
  uint32_t roundTrip;
  uint8_t rssi;
};

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  xbee.setSerial(Serial1);
  t_stamp_capacity = millis();

  Serial.println("Resetting EEPROM");
  //for (int i = 0; i < EEPROM.length(); i++) {
  //    EEPROM.write(i, 'A');
  //}
  Serial.println("Done resetting EEPROM");
  Serial.println("EEPROM has space for " + String(EEPROM.length() / sizeof(Data)) + " entries!");
  Serial.println("Entry size: " + String(sizeof(Data)));
}

void loop() {
  uint8_t rssi = 0;

  // start transmitting after a startup delay.  Note: this will rollover to 0 eventually so not best way to handle
  if (millis() - start > 5000) {
    // break down 10-bit reading into two bytes and place in payload
    for (int i = 0; i < 2; i++) {
      if (route[loopCnt][i] != 0) {
        payload[0] = route[loopCnt][i];
        xbee.send(tx);
      }
    }
    t_stamp_latency = micros();

    // TODO: SOMEHOW WITH MRAA ON THE OTHER END WE DONT GET A RESPONSE YET :-(

    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(5000)) {
        // got a response!

        // should be a znet tx status
        if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
            xbee.getResponse().getTxStatusResponse(txStatus);

            // get the delivery status, the fifth byte
            if (txStatus.getStatus() == SUCCESS) {
                Serial.println("[INFO] Got successful response!");
                succ++;
            } else {
                Serial.println("[ERROR] Response is not SUCCESS!");
                loss++;
            }

            xbee.getResponse().getRx16Response(rx16);
            rssi = rx16.getRssi();
        }
    } else if (xbee.getResponse().isError()) {
        //nss.print("Error reading packet.  Error code: ");
        //nss.println(xbee.getResponse().getErrorCode());
        // or flash error led
        Serial.println("[ERROR] Response is error!");
        loss++;
    } else {
        // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
        //flashLed(errorLed, 2, 50);
        Serial.println("[ERROR] No response!");
        loss++;
    }

    if (millis() - t_stamp_capacity > 1000) {
      uint32_t roundTrip = (micros() - t_stamp_latency);

      Serial.println("###############");
      Serial.println("RoundTrip: " + String(roundTrip) + " microseconds");
      Serial.println("Rssi: -" + String(rssi));
      Serial.println(
        "PackageErrorRate: " + String((loss * 1.0) / (loss + succ)) + " (" + loss + "/" + (loss + succ) +
        ")");

      Data data = {
        succ,
        loss,
        roundTrip,
        rssi
      };

      succ = 0;
      loss = 0;

      //EEPROM.put(eeAddress, data);
      //eeAddress += sizeof(Data);
    }
    delay(5000);
    loopCnt = (loopCnt + 1) % n;
    t_stamp_capacity = millis();
  }
}

