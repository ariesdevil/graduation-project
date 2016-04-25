#!/bin/bash

success=`grep 1 log | wc -l`
failure=`grep 0 log | wc -l`

echo "$success / ($success + $failure)" | bc -l
