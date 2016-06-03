#!/bin/bash

raspivid -w 640 -h 480 -fps 30 -t 0 -hf -vf -o - |
./test_UDPRawServer 192.168.1.105 192.168.1.100 2> log_r |
./test_UDPEncodedServer 192.168.1.105 192.168.1.100 > /dev/null 2> log_e  
