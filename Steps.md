# Steps to conduct the performance comparison

## Enable SGX in-kernel driver

```
./sgx_driver_conf.sh 
```

## Run Occlum

```
./run_occlum.sh
```

You will be in a occlum container instance.

```
cd /root/occlum/
```

You will see `pipe` and `spawn` among those examples.


### Run pipe

```
cd pipe
./run_pipe.sh
```

### Run spawn

```
cd spawn
./run_spawn.sh
```

## Run Graphene

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

Then, run the SGX application

```
make SGX=1 check
```

