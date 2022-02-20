#/bin/bash!

sudo docker rm -f graphene-sgx
sudo docker run -it --privileged -v /dev/sgx:/dev/sgx --name "graphene-sgx" liuweijie/graphene:v1.1
