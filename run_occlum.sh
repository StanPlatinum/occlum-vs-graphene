sudo docker rm -f occlum-sgx
sudo docker run --cap-add sys_ptrace -it --name occlum-sgx --device /dev/sgx/enclave --device /dev/sgx/provision liuweijie/occlum:v0.26
