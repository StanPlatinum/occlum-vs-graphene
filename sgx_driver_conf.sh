#/bin/bash!

sudo mkdir -p /dev/sgx
sudo ln -sf /dev/sgx_enclave /dev/sgx/enclave
sudo ln -sf /dev/sgx_provision /dev/sgx/provision
