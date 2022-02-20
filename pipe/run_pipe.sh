#!/bin/bash
set -e

SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
bomfile=${SCRIPT_DIR}/pipe.yaml

rm -rf occlum-instance
occlum new occlum-instance
cd occlum-instance

new_json="$(jq '.env.default = [ "OCCLUM=yes", "PWD=/bin/" ]' Occlum.json)" && \
    echo "${new_json}" > Occlum.json

rm -rf image
copy_bom -f $bomfile --root image --include-dir /opt/occlum/etc/template

occlum build
echo -e "\nBuild done. Running..."
occlum run  /bin/pipe_throughput 16
occlum run  /bin/pipe_throughput 32
occlum run  /bin/pipe_throughput 64
