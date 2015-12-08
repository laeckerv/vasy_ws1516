/*
 * Author: Jessica Gomez <jessica.gomez.hernandez@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file
 * @ingroup basic
 * @brief On board LED blink C++
 *
 * Blink the on board LED by writing a digital value to an output pin
 * using the IO mraa library.
 *
 * @date 29/09/2015
 */

#include "mraa.hpp"

#include <csignal>
#include <iostream>
#include <unistd.h>

/*
 * On board LED blink C++ example
 *
 * Demonstrate how to blink the on board LED, writing a digital value to an
 * output pin using the MRAA library.
 * No external hardware is needed.
 *
 * - digital out: on board LED
 *
 * Additional linker flags: none
 */

void handle(int sig) {
	mraa::Pwm* pwm_pina = new mraa::Pwm(0);
	mraa::Pwm* pwm_pinb = new mraa::Pwm(14);
	std::cout << "Signal detected: " << sig << std::endl;
	pwm_pina->config_percent(1, 0);
	pwm_pinb->config_percent(1, 0);

	exit(0);
}

void stop() {
	mraa::Pwm* pwm_pina = new mraa::Pwm(0);
	mraa::Gpio* ina1_pin = new mraa::Gpio(45);
	mraa::Gpio* ina2_pin = new mraa::Gpio(46);

	ina1_pin->write(1);
	ina2_pin->write(1);
	pwm_pina->config_percent(1, 1);

	usleep(1500000);

	ina1_pin->write(0);
	ina2_pin->write(0);
	pwm_pina->config_percent(1, 0);
}

void onem_straight(bool forward) {
	mraa::Pwm* pwm_pina = new mraa::Pwm(0);
	mraa::Gpio* ina1_pin = new mraa::Gpio(45);
	mraa::Gpio* ina2_pin = new mraa::Gpio(46);

	ina1_pin->write(forward);
	ina2_pin->write(!forward);
	pwm_pina->config_percent(1, 0.2);

	usleep(3600000);
	stop();
}




void left(int usec) {
}

void right(int usec) {
}

int main() {
	signal(SIGINT,handle);

	// define pwm pins; 
	// motor a is for going forward and backward
	//motor b is for steering the wheels
	mraa::Pwm* pwm_pina = new mraa::Pwm(0);
	mraa::Pwm* pwm_pinb = new mraa::Pwm(14);

	// check and enable pins
	if (pwm_pina == NULL) {
		std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}
	// enable PWM on the selected pin
	if (pwm_pina->enable(true) != MRAA_SUCCESS) {
		std::cerr << "Cannot enable PWM on mraa::PWM object, exiting"
				<< std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}
	if (pwm_pinb == NULL) {
		std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}
	// enable PWM on the selected pin
	if (pwm_pinb->enable(true) != MRAA_SUCCESS) {
		std::cerr << "Cannot enable PWM on mraa::PWM object, exiting"
				<< std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}

	// define in and  and standy pins
	mraa::Gpio* stby_pin = new mraa::Gpio(15);
	mraa::Gpio* ina1_pin = new mraa::Gpio(45);
	mraa::Gpio* ina2_pin = new mraa::Gpio(46);
	mraa::Gpio* inb1_pin = new mraa::Gpio(47);
	mraa::Gpio* inb2_pin = new mraa::Gpio(48);

	// check an enable the pins
	if (stby_pin == NULL) {
		std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}
	// set the pin as output
	if (stby_pin->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
		std::cerr << "Can't set digital pin as output, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}

	if (ina1_pin == NULL) {
		std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}
	// set the pin as output
	if (ina1_pin->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
		std::cerr << "Can't set digital pin as output, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}

	if (ina2_pin == NULL) {
		std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}
	// set the pin as output
	if (ina2_pin->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
		std::cerr << "Can't set digital pin as output, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}

	if (inb1_pin == NULL) {
		std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}
	// set the pin as output
	if (inb1_pin->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
		std::cerr << "Can't set digital pin as output, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}
	if (inb2_pin == NULL) {
		std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}
	// set the pin as output
	if (inb2_pin->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
		std::cerr << "Can't set digital pin as output, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}

	// set the motor action control pins
	stby_pin->write(0);	// standby=0 means motor off
	ina1_pin->write(0); // a to 0/1 means go forward
	ina2_pin->write(1);
	inb1_pin->write(1); // b to 1/0 means go left/right (try out)
	inb2_pin->write(0);

	// PWM duty_cycle value, 0.0 == 0%, 1.0 == 100%
	float duty_cycle = 0.0;

	// select a pulse width period of 1ms
	int period = 1;

	float dir = 0.0;

	// turn on motor driver
	stby_pin->write(1);

	stop();
	onem_straight(true);
	onem_straight(true);
	onem_straight(true);

	sleep(2);
	onem_straight(false);
	onem_straight(false);
	onem_straight(false);


	// this makes the car go forward or backward with low speed
	//std::cout << "Testdrive" << std::endl; 
	//pwm_pina->config_percent(period, 0.3);
	//pwm_pinb->config_percent(period*2, 0.1);

	//// Drive straight
	//sleep(6);

	//// Drive left
	//inb1_pin->write(0);
	//inb2_pin->write(1);
	//pwm_pinb->config_percent(period, 1);
	//usleep(1500000); 

	//// Drive straight
	//pwm_pinb->config_percent(period, 0);
	//sleep(4);

	// Stop
	pwm_pina->config_percent(period, 0);
	pwm_pinb->config_percent(period, 0);
	return mraa::SUCCESS;
}
