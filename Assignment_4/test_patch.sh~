#!/bin/bash                                                                     

tmp=$(mktemp -d)
cd $tmp
touch -d '1918-11-11 11:00 GMT' wwi-armistice
touch now
sleep 1
touch now1
TZ=UTC0 ~/Fall2017/CS_35L/Assignment_4/coreutils-with-bug/bin/ls -lt --full-time wwi-armistice now now1
cd ~/Fall2017/CS_35L/Assignment_4/
rm -rf $tmp
