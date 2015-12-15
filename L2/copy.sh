#!/bin/bash

IP=192.168.42.1

scp Motor.cpp Makefile root@${IP}:/home/root/
ssh root@${IP} 'make'
