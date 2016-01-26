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
#include <sstream>
#include <thread>
#include <vector>
#include <algorithm>

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

using namespace std;

mraa::Pwm *pwm_engine = new mraa::Pwm(0);
mraa::Pwm *pwm_steering = new mraa::Pwm(14);
mraa::Gpio *pin_standby = new mraa::Gpio(15);
mraa::Gpio *pin_backward = new mraa::Gpio(45);
mraa::Gpio *pin_forward = new mraa::Gpio(46);
mraa::Gpio *pin_right = new mraa::Gpio(47);
mraa::Gpio *pin_left = new mraa::Gpio(48);

static const int cycle = 800000;

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

void stop(void) {
    pin_forward->write(1);
    pin_backward->write(1);
    usleep(500000);
    pin_forward->write(0);
    pin_backward->write(0);
}

void handle(int sig) {
    reset_Engine();
    reset_Steering();
    exit(0);
}

void onem_straight(bool forward) {
    pin_forward->write(forward);
    pin_backward->write(!forward);
    pwm_engine->config_percent(1, 0.2);
    usleep(3600000);
    stop();
}

void straight(bool forward) {
    pin_forward->write(forward);
    pin_backward->write(!forward);
    pwm_engine->config_percent(1, 0.4);
    usleep(cycle);
    stop();
}

void straight_left(bool forward) {
    pin_forward->write(forward);
    pin_backward->write(!forward);
    pwm_engine->config_percent(1, 0.4);

    pin_left->write(1);
    pin_right->write(0);
    pwm_steering->config_percent(1, 1);

    usleep(cycle);
    stop();
    reset_Steering();
}

void straight_right(bool forward) {
    pin_forward->write(forward);
    pin_backward->write(!forward);
    pwm_engine->config_percent(1, 0.4);

    pin_left->write(0);
    pin_right->write(1);
    pwm_steering->config_percent(1, 1);

    usleep(cycle);
    stop();
    reset_Steering();
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
        cerr << "Can't create mraa::Pwm object, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pwm_engine->enable(true) != MRAA_SUCCESS) {
        cerr << "Cannot enable PWM on mraa::PWM object, exiting"
        << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pwm_steering == NULL) {
        cerr << "Can't create mraa::Pwm object, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pwm_steering->enable(true) != MRAA_SUCCESS) {
        cerr << "Cannot enable PWM on mraa::PWM object, exiting"
        << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_standby == NULL) {
        cerr << "Can't create mraa::Pwm object, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_standby->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        cerr << "Can't set digital pin as output, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_forward == NULL) {
        cerr << "Can't create mraa::Pwm object, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_forward->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        cerr << "Can't set digital pin as output, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_backward == NULL) {
        cerr << "Can't create mraa::Pwm object, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_backward->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        cerr << "Can't set digital pin as output, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_left == NULL) {
        cerr << "Can't create mraa::Pwm object, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_left->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        cerr << "Can't set digital pin as output, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_right == NULL) {
        cerr << "Can't create mraa::Pwm object, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }
    if (pin_right->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
        cerr << "Can't set digital pin as output, exiting" << endl;
        return MRAA_ERROR_UNSPECIFIED;
    }

    pin_standby->write(0);    // standby=0 means motor off
    reset_Engine();
    reset_Steering();
    pin_standby->write(1);

    return mraa::SUCCESS;
}

int handleMessage(int input) {
    if(input != 0)
        cout << "[INFO] Got data: " << input;

    /*
     * forward = 1
     * backward = 2
     * left = 3
     * right = 4
     * stop = 5
     */

    switch(input) {
        case 0:
            break;
        case 1:
            cout << " forward" << endl;
            straight(true);
            break;
        case 2:
            cout << " backward" << endl;
            straight(false);
            break;
        case 3:
            cout << " left" << endl;
            straight_left(true);
            break;
        case 4:
            cout << " right" << endl;
            straight_right(true);
            break;
        case 5:
            cout << " stop" << endl;
            stop();
            break;
        default: {
            cout << "[ERROR] Got an invalid commmand!" << endl;
            handle(SIGINT);
        }
    }

    return 0;
}

void clean(mraa::Uart *dev) {
   while(dev->dataAvailable(100))
       dev->readStr(1);
}

int sendCmd(mraa::Uart *dev, string cmd, bool response = false) {
    int c_max = 32;
    int i = 0;
    char rssi[32];
    for(int j = 0; j < c_max; j++)
        rssi[j] = 0;

    clean(dev);
    dev->flush();
    dev->writeStr(cmd);
    while(dev->dataAvailable(1500) && i < c_max) {
        dev->read(&rssi[i],1);
        if(rssi[i] == 13) {
            rssi[i] = 0;
            break;
        }
        i++;
    }

    if (strcmp(rssi, "OK") == 0) {
        return 1;
    }
    else if (response) {
        stringstream ss;
        char * p_end;
        ss << "0" << "x" << rssi;
        dev->writeStr(ss.str() + "\r\n");
        return strtol(ss.str().c_str(),&p_end,16);
    }
    else {
        return 0;
    }
}

int main() {
    signal(SIGINT, handle);

    int ret = 0;
    if ((ret = init()) != 0)
        return ret;

    mraa::Uart *dev;
    try {
        dev = new mraa::Uart(0);
    } catch (exception &e) {
        cout << "[ERROR]" << e.what() << ", likely invalid platform config" << endl;
    }

    if (dev->setBaudRate(9600) != mraa::SUCCESS) {
        cout << "[ERROR] Setting parity on UART" << endl;
    }

    if (dev->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
        cout << "[ERROR] Setting parity on UART" << endl;
    }

    if (dev->setFlowcontrol(false, false) != mraa::SUCCESS) {
        cout << "[ERROR] Setting flow control UART" << endl;
    }

    dev->writeStr("I'm waiting for data!");
    cout << "[INFO] Waiting for data" << endl;
    vector<thread> threads;
    while (true) {
        if (dev->dataAvailable(100000)) {
            char input = '0';
            dev->read(&input,sizeof(char));
            threads.push_back(thread(handleMessage, atoi(&input)));
        }

        cout << "[INFO] Reading rssi" << endl;
        if(sendCmd(dev, "+++")) {
            cout << "[INFO] RSSI: -" << sendCmd(dev, "ATDB\r", true) << endl;
            sendCmd(dev, "ATCN\r");
        }
        cout << "[INFO] Reading rssi done" << endl;
    }

    for_each(threads.begin(), threads.end(), [](thread &t) { t.join(); });

    cout << "[INFO] Done - shutting down!" << endl;
    delete dev;

    // Reset
    handle(SIGINT);
}

