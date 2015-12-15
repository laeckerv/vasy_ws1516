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

static mraa::Pwm* pwm_steering = new mraa::Pwm(0);
static mraa::Pwm* pwm_engine = new mraa::Pwm(14);
static mraa::Gpio* pin_standby = new mraa::Gpio(15);
static mraa::Gpio* pin_forward = new mraa::Gpio(45);
static mraa::Gpio* pin_backward = new mraa::Gpio(46);
static mraa::Gpio* pin_left = new mraa::Gpio(47);
static mraa::Gpio* pin_right = new mraa::Gpio(48);

void handle(int sig) {
	pwm_engine->config_percent(1, 0);
	pwm_steering->config_percent(1, 0);
	exit(0);
}

void reset_Engine() {
    pin_forward->write(0);
    pin_backward->write(0);
    pwm_engine->config_percent(1, 0);
}

void reset_Steering() {
    pin_left->write(0);
    pin_right->write(0);
    pwm_steering->config_percent(1, 0);
}

void stop() {
	pin_forward->write(1);
	pin_backward->write(1);
	pwm_engine->config_percent(0.5f, 1);

    sleep(2);
	//usleep(1500000);

    reset_Engine();
}

void onem_straight(bool forward) {
	pin_forward->write(forward);
	pin_backward->write(!forward);
	pwm_engine->config_percent(1, 0.2);

	usleep(3600000);
	stop();
}

void left(int usec) {
    pin_left->write(1);
    pin_right->write(0);
    pwm_steering->config_percent(1, 1);
    usleep(usec);

    reset_Steering();
}

void right(int usec) {
    pin_left->write(0);
    pin_right->write(1);
    pwm_steering->config_percent(1, 1);
    usleep(usec);

    reset_Steering();
}

int init() {
    if (pwm_engine == NULL) {
        std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pwm_engine->enable(true) != MRAA_SUCCESS) {
        std::cerr << "Cannot enable PWM on mraa::PWM object, exiting"
        << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pwm_steering == NULL) {
        std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pwm_steering->enable(true) != MRAA_SUCCESS) {
        std::cerr << "Cannot enable PWM on mraa::PWM object, exiting"
        << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_standby == NULL) {
        std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_standby->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        std::cerr << "Can't set digital pin as output, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_forward == NULL) {
        std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_forward->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        std::cerr << "Can't set digital pin as output, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_backward == NULL) {
        std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_backward->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        std::cerr << "Can't set digital pin as output, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_left == NULL) {
        std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_left->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        std::cerr << "Can't set digital pin as output, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_right == NULL) {
        std::cerr << "Can't create mraa::Pwm object, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_right->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        std::cerr << "Can't set digital pin as output, exiting" << std::endl;
        return MRAA_ERROR_UNSPECIFIED;
    }

    pin_standby->write(1);	// standby=1 means motor on
    reset_Engine();
    reset_Steering();

    return mraa::SUCCESS;
}

int main() {
    int ret = 0;

	signal(SIGINT,handle);

    if((ret = init()) != 0)
        return ret;

    sleep(2);
    std::cout << "Breaking now" << std::endl;
	stop();
    std::cout << "Done breaking" << std::endl;
	// onem_straight(true);
	// onem_straight(true);
	// onem_straight(true);

	// sleep(2);
	// onem_straight(false);
	// onem_straight(false);
	// onem_straight(false);

	// // Reset
    reset_Engine();
    reset_Steering();

	return 0;
}
