#!/bin/bash
kill $(ps | grep 'vlc\|test' | awk '{print $1}') 
