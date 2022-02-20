#!/usr/bin/env bash

cd ../spawn
gcc -O3 -fPIC spawn_child.c -o spawn_child 
gcc -O3 -fPIC spawn_victim.c -o spawn_victim
./run_spawn.sh

