#!/bin/bash

IP=192.168.42.1

DATE=$(date "+%m%d%H%M%Y")
ssh root@${IP} "date $DATE"
scp Motor.cpp Makefile root@${IP}:/home/root/
ssh root@${IP} 'make'
