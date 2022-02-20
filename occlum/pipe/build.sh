#!/usr/bin/env bash

gcc -O3 -fPIC pipe_throughput.c -o pipe_throughput 
gcc -O3 -fPIC dev_null.c -o dev_null
./run_pipe.sh

