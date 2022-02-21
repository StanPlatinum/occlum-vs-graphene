# Steps to conduct the performance comparison

## Enable SGX in-kernel driver

```
./sgx_driver_conf.sh 
```

## Run Occlum

```
./run_occlum.sh
```

Press ENTER, you will be in a occlum container instance.

```
cd occlum-vs-graphene
cd occlum
```

You will see `pipe` and `spawn` among those examples.


### Run pipe

```
cd pipe
./build.sh
```

### Run spawn

```
cd spawn
./build.sh
```

## Run Graphene

Graphene should be run in another Docker container.

```
./run_graphene.sh
```

You will be in a graphene container instance.

```
cd /root/graphene/Examples/
```

You will see `pipe` and `spawn` among those examples.

### Run pipe

In the `/root/graphene/Examples/` directory, entering the `pipe` dir

```
cd pipe
```

To build the SGX application, run

```
make clean && make SGX=1 
```

Then, run the SGX application

```
make SGX=1 check
```

### Run spawn

In the `/root/graphene/Examples/` directory, entering the `spawn` dir

```
cd spawn
```

To build the SGX application, run

```
make clean && make SGX=1 
```

Then, run the SGX application. This test will last for about 10 minutes.

```
make SGX=1 check
```

