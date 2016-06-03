#!/bin/bash

./test_UDPEncodedClient 192.168.1.105 192.168.1.100  2> log_e |
tee with-encode.h264 |
cvlc --demux=h264 - &

./test_UDPRawClient 192.168.1.105 192.168.1.100 2> log_r |
tee without-encode.h264 |
cvlc --demux=h264 - &
