# Test Application

The test application is made up of a control thread that performs random pointer chasing to simulate a workload and measures wake up jitter, execution time and cache misses.
The application also features a statistics handler thread that processes and logs timing information.
The statistics are either output to the console via stdout or published to an MQTT broker.

```{figure} images/rt_linux_tutorial.svg
   :width: 100%
   :alt: High-level design of rt app

   High-level Design of the test application
```

```{note}
The pointer chasing buffer size should exceed the L2 cache size of your processor to see an improvement by cache partitioning described later in this tutorial.
You can change the buffer size in `rt_linux_tutorial.c` by editing the `WORKLOAD_BUFFER_SIZE` definition.
```


## Build the docker image for the test application

The source code for the test application is available in the code archive you downloaded earlier.

First make any changes to the C program code, like modifying the `WORKLOAD_BUFFER_SIZE`.
Then build the Docker image by running the following command in the directory containing `Dockerfile`:

```bash
cd tutorial-intel-tcc-code/docker
docker build -t intel_tcc_tutorial_image .
```

To verify that the image was built successfully, list all Docker images:

```bash
docker images
```
You should see `intel_tcc_tutorial_image` listed among the images.

The `rt_linux_tutorial` application can be started by running the Docker container in interactive mode:

```bash
 docker run -it --privileged --rm --network docker-compose_stats intel_tcc_tutorial_image

./rt_linux_tutorial -h
    Options:
      -h        Display this help message
      -i <time> Set the cycle time of the control thread in microseconds - default is 500us
      -s <0|1>  Set the output method for statistics (0 for stdout, 1 for MQTT to localhost) - default stdout
```