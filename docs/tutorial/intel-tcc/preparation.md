# Preparation

This sections outlines the steps and components required to set up the system and build the test application.

## Dependencies

- **MSR Tools**: A driver and utilities to read and write Model-Specific Registers.
- **Paho MQTT C Client Library**: A client library for MQTT messaging in C applications.
- **cJSON Library**: A ultra-lightweight JSON parser in ANSI C.
- **Telegraf**: An agent for collecting and reporting metrics and data.
- **InfluxDB**: A time-series database designed to handle high write and query loads.
- **Grafana**: An open-source platform for monitoring and observability that allows you to visualize data.

## Setup Steps

### Enable Intel® TCC Mode

With the Intel® reference BIOS the <span style="font-family: 'Courier New';">Intel® TCC Mode</span> can be enabled under <span style="font-family: 'Courier New';">"Intel® Advanced Menu > Time Coordinated Computing"</span>.
Talk to your board vendor if the <span style="font-family: 'Courier New';">Intel® TCC Mode</span> is not visible or follow the steps listed in the "Intel® Core Processors Firmware Configuration" section of [TCC User Guide](https://www.intel.com/content/www/us/en/content-details/831067/public-intel-time-coordinated-compute-tcc-user-guide.html) and set the options manually.

### Install Ubuntu OS
1. Install the latest [Ubuntu 24.04 LTS Desktop](https://releases.ubuntu.com/noble/). 
   Refer to [Ubuntu Desktop installation tutorial](https://ubuntu.com/tutorials/install-ubuntu-desktop#1-overview) if needed.

2. Install the Ubuntu real-time kernel.
  Real-time Ubuntu is available via Ubuntu Pro.
  A free [Ubuntu Pro account](https://ubuntu.com/pro/dashboard) is available for personal and small-scale commercial use.
  With a free Ubuntu Pro subscription you get five personal tokens which can be used for up to five machines.
  The kernel can simply be installed by running these commands:
  
  ```bash
  sudo pro attach
  sudo pro enable realtime-kernel
  sudo reboot
  ```

Refer to [How to enable Real-time Ubuntu](https://canonical-ubuntu-pro-client.readthedocs-hosted.com/en/latest/howtoguides/enable_realtime_kernel/) for more details.

### Other required software

Install other required tools:
```bash
sudo apt update
sudo apt install msr-tools
sudo apt install stress-ng
```

```{note}
For the rest of the required software you can either use Docker, or manually setup everything yourself.
```

`````{tabs}
````{group-tab} Using Docker

### Install Docker and Docker Compose 

Please follow the steps described in [Install Docker Engine on Ubuntu](https://docs.docker.com/engine/install/ubuntu/) to install docker.

### Start the Grafana Statistics Infrastructure

Download the source code archive [here](intel-tcc-tutorial.tar.gz).

```bash
tar -xvf intel-tcc-tutorial.tar.gz --one-top-level
cd intel-tcc-tutorial/docker/docker-compose
docker compose up -d 

# Verify the telegraf, grafana, influxdb and mosquitto containers are up and running
docker compose ps
```

Once the containers are up and running, you can connect to Grafana by following these steps:
- Open your preferred web browser.
- Enter the following URL in the address bar: [http://localhost:3000/](http://localhost:3000/)
  - If you are running Docker on a remote server, replace `localhost` with the server's IP address or domain name.
    Also make sure port 3000 is exposed through your firewall.

- Log In to Grafana: You will be prompted to log in. Use the following credentials:
  - Username: admin
  - Password: admin1

You should see a similar dashboard like the screenshot in {doc}`results`.
If the dashboard is not visible by default, you should find the `rt_linux_tutorial` dashboard under `Provisioned Dashboards` in the Dashboards menu.

### Build the docker image for the RT Linux Tutorial

First make any changes to the C program code, like modifying the `WORKLOAD_BUFFER_SIZE`.

Build the Docker image by running the following command in the directory containing `Dockerfile`:

```bash
cd intel-tcc-tutorial/docker
docker build -t rt_linux_tutorial_image .
```

To verify that the image was built successfully, list all Docker images:

```bash
docker images
```
You should see `rt_linux_tutorial_image` listed among the images.

The `rt_linux_tutorial` application can be started by running the Docker container in interactive mode:

```bash
 docker run -it --privileged --rm --network docker-compose_stats rt_linux_tutorial_image

./rt_linux_tutorial -i 250 -s 1

```

````
````{group-tab} Manual setup

#### Install Paho MQTT C Client Library and cJSON

```bash
sudo apt install git build-essential
# Install the Paho MQTT C client library https://github.com/eclipse/paho.mqtt.c.git 
sudo apt install libpaho-mqtt-dev
# Install cJSON https://github.com/DaveGamble/cJSON.git 
sudo apt install libcjson-dev
```
### Build the Application

Download the source code archive [here](intel-tcc-tutorial.tar.gz).
Then extract it.

```bash
tar -xvf intel-tcc-tutorial.tar.gz --one-top-level
cd intel-tcc-tutorial
```

Make any required changes to the source code like buffer sizes.
Most likely you will need to change the MQTT address in `rt_linux_tutorial.c` to look like this:

```c
#define ADDRESS     "tcp://localhost:1883" // use localhost if app does not run in container
//#define ADDRESS     "tcp://mosquitto:1883"
```

Then compile it using the provided Makefile.

```bash
make
```

This will generate the executable named `rt_linux_tutorial`.

Run the application with superuser privileges to allow MSR access and to set thread affinities. 

```bash
sudo ./rt_linux_tutorial

-i <time> Set the cycle time of the control thread in microseconds - default is 500us
-s <0|1>  Set the output method for statistics (0 for stdout, 1 for MQTT to localhost) - default stdout
```

#### Install Mosquitto MQTT Broker

```bash
sudo apt install mosquitto mosquitto-clients
```

#### Install and Configure Telegraf

Install Telegraf by following their guide [here](https://docs.influxdata.com/telegraf/v1/install/#install-from-the-influxdata-repository).

Configure Telegraf to use the `mqtt_consumer` input plugin and the `influxdb` output plugin by editing `/etc/telegraf/telegraf.conf`.

For example by appending this to the config file:

```bash
# Read metrics from MQTT topic(s)
[[inputs.mqtt_consumer]]
  servers = ["tcp://localhost:1883"]
  topics = [
    "sensor/data",
  ]
  data_format = "json"
  json_time_key = ""  # Set if you have a timestamp in your JSON
  json_time_format = ""  # Set the format if you have a timestamp

# Write metrics to InfluxDB
[[outputs.influxdb]]
  urls = ["http://localhost:8086"] # or the URL to your InfluxDB instance
  database = "tcc_tutorial_data" # the database to write to
```

Then restart Telegraf:

```bash
sudo systemctl restart telegraf
```

#### Install and Set Up InfluxDB v1

Install and start InfluxDB v1 as explained in [this guide](https://docs.influxdata.com/influxdb/v1/introduction/install/#installing-influxdb-oss).

Create a new InfluxDB database.

```bash
influx -execute 'CREATE DATABASE tcc_tutorial_data'
```
#### Install and Configure Grafana OSS

Install Grafana OSS according to [their documentation](https://grafana.com/docs/grafana/latest/setup-grafana/installation/debian/#install-from-apt-repository).
Then start it using [these steps](https://grafana.com/docs/grafana/latest/setup-grafana/start-restart-grafana/#linux).

Access Grafana at e.g. [http://localhost:3000/](http://localhost:3000/) and log in with the default credentials (admin/admin).

#### Add InfluxDB as a Data Source in Grafana

In the Grafana UI, go to *Connections* > *Data Sources* > *Add Data Source* > *InfluxDB*.
Enter the connection details for your InfluxDB instance:

* URL: `http://localhost:8086`
* Database: `tcc_tutorial_data`

Click on *Save & test*

#### Create a Dashboard in Grafana

Download [this](code/docker/docker-compose/grafana-provisioning/dashboards/rt_linux_tutorial.json) file.

In Grafana go to *Dashboards* > *New* > *Import* and upload the file you just downloaded.

You will see a dashboard with three empty panels.
For each panel click on the three dots menu in the top right hand corner, and click *Edit*.
Click on the dropdown next to Data Source and select InfluxDB.
Click back and then save the dashboard.

#### Verifying the Data Flow

Run the `rt_linux_tutorial` application and configure it to publish statistics to MQTT:

```bash
sudo ./rt_linux_tutorial -s 1
```

* Subscribe to the MQTT topic and verify that messages are being published:

```bash
mosquitto_sub -t "#"
```

* Check Telegraf logs for any processing errors:

```bash
journalctl -f -u telegraf
```

* Query InfluxDB to confirm that data is being written.
* View the Grafana dashboard to see the visualized data.

#### Troubleshooting

* Ensure that all services (Mosquitto, Telegraf, InfluxDB, Grafana) are running.
* Check network connectivity between components.
* Verify configuration files for correct syntax and settings.
* Consult logs for each component for error messages.

#### Conclusion

By following these steps, you should have a fully functional data pipeline that collects data from a C application, sends it via MQTT, processes it with Telegraf, stores it in InfluxDB, and visualizes it with Grafana

````
`````