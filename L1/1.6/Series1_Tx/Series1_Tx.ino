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
This example is for Series 1 XBee
Sends a TX16 or TX64 request with the value of analogRead(pin5) and checks the status response for success
Note: In my testing it took about 15 seconds for the XBee to start reporting success, so I've added a startup delay
*/

XBee xbee = XBee();

unsigned long start = millis();

// allocate two bytes for to hold a 10-bit analog reading
uint8_t payload[] = { 0, 0};

// with Series 1 you can use either 16-bit or 64-bit addressing

// 16-bit addressing: Enter address of remote XBee, typically the coordinator
Tx16Request tx = Tx16Request(0x1DFA, payload, sizeof(payload));

// 64-bit addressing: This is the SH + SL address of remote XBee
// XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40F30184);
// unless you have MY on the receiving radio set to FFFF, this will be received as a RX16 packet
// Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();

int pin5 = 0;
int led = 13;
bool on = true;

unsigned long t_stamp = 0;
unsigned long sent = 0;
unsigned long loss = 0;
unsigned long packet_count = 0;

void flashLed(int brightness, int duration) {
    duration *= 10;
    analogWrite(led, brightness);
    delay(duration);
    analogWrite(led, 0);
    delay(duration);
}

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  xbee.setSerial(Serial1);
}

void loop() {
   int brightness = random(0,255);
   int duration = random(1,255);
   
   // start transmitting after a startup delay.  Note: this will rollover to 0 eventually so not best way to handle
    if (millis() - start > 5000) {
      // break down 10-bit reading into two bytes and place in payload
      payload[0] = brightness;
      payload[1] = duration;
      
      Serial.println("###### Sending ######");
      xbee.send(tx);
      packet_count++;
      t_stamp = micros();

      // after sending a tx request, we expect a status response
      // wait up to 5 seconds for the status response
      if (xbee.readPacket(5000)) {
          // got a response!
  
          // should be a znet tx status            	
      	if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE) {
      	   xbee.getResponse().getTxStatusResponse(txStatus);
      		
      	   // get the delivery status, the fifth byte
             if (txStatus.getStatus() == SUCCESS) {
                t_stamp = (micros() - t_stamp)/2;
                sent++;
                Serial.println("SUCCESS: " + String(t_stamp) + " microseconds");
                flashLed(brightness, duration);
             } else {
              	// the remote XBee did not receive our packet. is it powered on?
                loss++;
                Serial.println("NO SUCCESS");
             }
          }      
        } else if (xbee.getResponse().isError()) {
          //nss.print("Error reading packet.  Error code: ");  
          //nss.println(xbee.getResponse().getErrorCode());
          // or flash error led
          loss++;
          Serial.println("NO SUCCESS");
        } else {
          // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
          //flashLed(errorLed, 2, 50);
          loss++;
          Serial.println("NO SUCCESS");
        }
        Serial.println("Sent: " + String(sent) + "/" + String(packet_count));
        Serial.println("Loss:" + String(loss) + "/" + String(packet_count));
      }
    delay(1000);
}
