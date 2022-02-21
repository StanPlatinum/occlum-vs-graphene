#!/usr/bin/env bash

cd ../spawn
occlum-gcc -O3 -fPIC spawn_child.c -o spawn_child 
occlum-gcc -O3 -fPIC spawn_victim.c -o spawn_victim
./run_spawn.sh

