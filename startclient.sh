#!/bin/bash

./test_UDPRawClient  192.168.1.105 192.168.1.100 | cvlc --demux=h264 - &
./test_UDPEncodedClient  192.168.1.105 192.168.1.100 | cvlc --demux=h264 - &
