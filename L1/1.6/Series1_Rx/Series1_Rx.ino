/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <XBee.h>

/*
This example is for Series 1 XBee (802.15.4)
Receives either a RX16 or RX64 packet and sets a PWM value based on packet data.
Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
Rx16Response rx16 = Rx16Response();
Rx64Response rx64 = Rx64Response();

int led = 13;

uint8_t option = 0;
uint8_t brightness = 0;
uint8_t duration = 0;

void flashLed(int brightness, int duration) {
    duration *= 10;
    analogWrite(led, brightness);
    delay(duration);
    analogWrite(led, 0);
    delay(duration);
}

void setup() {
  pinMode(led, OUTPUT);
  
  // start serial
  Serial.begin(9600);
  Serial1.begin(9600);
  xbee.setSerial(Serial1);
}

// continuously reads packets, looking for RX16 or RX64
void loop() {
    
    xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
      // got something
      Serial.println("Message available");
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE || xbee.getResponse().getApiId() == RX_64_RESPONSE) {
        // got a rx packet
        
        if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
                xbee.getResponse().getRx16Response(rx16);
        	option = rx16.getOption();
        	brightness = rx16.getData(0);
          duration = rx16.getData(1);
          Serial.println(rx16.getRssi());
        } else {
                xbee.getResponse().getRx64Response(rx64);
        	option = rx64.getOption();
        	brightness = rx64.getData(0);
          duration = rx64.getData(1);
        }
        
        // TODO check option, rssi bytes    
        //flashLed(led, 1, 10)
        
        // switch LED
        if(brightness && duration)
          flashLed(brightness, duration);
           
      } else {
      	// not something we were expecting
        Serial.println("Failed");  
      }
    } else if (xbee.getResponse().isError()) {
      //nss.print("Error reading packet.  Error code: ");  
      //nss.println(xbee.getResponse().getErrorCode());
      // or flash error led
      Serial.println("Error");
    } 
}
