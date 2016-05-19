#!/bin/bash

./test_UDPEncodedClient 192.168.1.105 192.168.1.100  2> log |
vlc --demux=h264 -
