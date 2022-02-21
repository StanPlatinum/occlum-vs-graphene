#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
bomfile=${SCRIPT_DIR}/spawn.yaml

rm -rf occlum-instance
occlum new occlum-instance
cd occlum-instance

new_json="$(jq '.resource_limits.user_space_size = "8192MB" |
        .process.default_heap_size = "128KB" |
        .process.default_stack_size = "128KB" |
        .env.default = [ "OCCLUM=yes"]' Occlum.json)" && \
    echo "${new_json}" > Occlum.json

rm -rf image
copy_bom -f $bomfile --root image --include-dir /opt/occlum/etc/template

occlum build
echo -e "\nBuild done. Running..."
occlum run /bin/spawn_child 100 0
