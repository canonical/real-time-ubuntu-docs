# The test application

The test application is named **rt_linux_tutorial**.
Its architecture consist of a control thread that performs random pointer chasing to simulate a workload and measures wake up jitter, execution time and cache misses.
The application also features a statistics handler thread that processes and logs timing information.
The statistics are either output to the console via STDOUT or published to an MQTT broker.

```{figure} images/rt_linux_tutorial.svg
   :width: 100%
   :alt: High-level design of rt app

   High-level design of the test application
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
cd tutorial-intel-tcc-code
docker build -t intel_tcc_tutorial_image .
```

To verify that the image was built successfully, list all Docker images:

```bash
docker images
```
You should see `intel_tcc_tutorial_image` listed among the images.

Run a new interactive container using this image:

```bash
docker run -it --privileged --rm --network docker-compose_stats intel_tcc_tutorial_image
```

Inside the container the `rt_linux_tutorial` application can be started:

```bash
root@container:~# ./rt_linux_tutorial -h
Usage: rt_linux_tutorial [OPTIONS]
Options:
  -h        Display this help message
  -i <time> Set the cycle time of the control thread in microseconds - default is 500us
  -s <0|1>  Set the output method for statistics (0 for stdout, 1 for MQTT to localhost) - default stdout
```

Run it with MQTT support.

```bash
./rt_linux_tutorial -i 1000 -s 1
```

Data should start appearing on the Grafana dashboard.

## Troubleshooting

If you don't see any data on the Grafana dashboard, try the following to debug the data flow.

### Subscribe to the Mosquitto broker
```
mosquitto_sub -t '#'
```
You should see a continuous stream of JSON data, that looks similar to this:
```json
{"exec_time":170852,"wakeup_jitter":6592,"cache_misses":0,"ipc":0.036748599261045456}
```

### Check Telegraf logs
```
cd tutorial-intel-tcc-code/docker-compose/
docker compose logs -f telegraf
```
You should see the startup sequence of Telegraf, loading InfluxDB as output plugin and MQTT as input.
There should not be any errors in the log.
```
telegraf | I! Loaded outputs: influxdb
...
telegraf | I! [inputs.mqtt_consumer] Connected [tcp://mosquitto:1883]
```

### Check InfluxDB logs
```
cd tutorial-intel-tcc-code/docker-compose/
docker compose logs -f influxdb
```
InfluxDB prints logs for all queries that are done on the database.
If the Grafana dashboard is open, you should see new logs appearing every 5 seconds, showing the queries made by Grafana.
Data that is written to the database is also logged.
```
influxdb | "GET /query?db=tcc_tutorial_data&epoch=ms&q=SELECT+last%28%22cache_misses%22%29+FROM+%22mqtt_consumer%22+WHERE+time+%3E%3D+1740395727546ms+and+time+%3C%3D+1740396027546ms+GROUP+BY+time%28200ms%29+fill%28null%29+ORDER+BY+time+ASC HTTP/1.1" 200 3898 "-" "Grafana/11.5.2" 5995cb04-f2a1-11ef-8488-0242ac120002 10180
influxdb | "POST /write?db=tcc_tutorial_data HTTP/1.1 " 204 0 "-" "Telegraf/1.33.2 Go/1.23.5" 5a0a00b6-f2a1-11ef-8489-0242ac120002 14423
```