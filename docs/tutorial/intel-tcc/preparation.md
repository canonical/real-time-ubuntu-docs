# Preparation

This sections outlines the preparation steps for following this tutorial.
It explains which software components are required and how to set it up.


## Install Real-time Ubuntu

1. Install the latest [Ubuntu 24.04 LTS](https://releases.ubuntu.com/noble/) Desktop or Server.
   Refer to [Ubuntu Desktop installation tutorial](https://ubuntu.com/tutorials/install-ubuntu-desktop#1-overview) if needed.

2. Install the Ubuntu real-time kernel.
   Real-time Ubuntu is available via [Ubuntu Pro](https://ubuntu.com/pro/dashboard).
   The real-time kernel can be installed by running these commands:

   ```bash
   sudo pro attach
   sudo pro enable realtime-kernel
   ```

   Refer to [How to enable Real-time Ubuntu](https://canonical-ubuntu-pro-client.readthedocs-hosted.com/en/latest/howtoguides/enable_realtime_kernel/) for more details.


## Other required software

Install required tools:
```bash
sudo apt update
sudo apt install msr-tools
sudo apt install stress-ng
```

Download the example code archive [here](tutorial-intel-tcc-code.tar.gz).
Then extract it.

```bash
tar -xvf tutorial-intel-tcc-code.tar.gz --one-top-level
```

## Statistics infrastructure

This tutorial uses Mosquitto MQTT, Telegraf, InfluxDB and Grafana to capture and visualize the statistics obtained from the test application.
These tools can be run as Docker containers, making the installation quicker and easier.

### Install Docker and Docker Compose 

Please follow the steps described in [Install Docker Engine on Ubuntu](https://docs.docker.com/engine/install/ubuntu/) to install docker.

### Start the services

The docker-compose definition of the statistics infrastructure is provided in the example code archive you downloaded earlier.
Navigate into the `docker-compose` subdirectory, and then execute docker compose:

```bash
cd tutorial-intel-tcc-code/docker/docker-compose
docker compose up -d 
```

Verify that the Telegraf, Grafana, InfluxDB and Mosquitto containers are up and running:

```
docker compose ps
```

If everything is running, you can connect to Grafana by following these steps:
- Open your preferred web browser.
- Enter the following URL in the address bar: [http://localhost:3000/](http://localhost:3000/)
  - If you are running Docker on a remote server, replace `localhost` with the server's IP address or domain name.
    Also make sure port 3000 is exposed through your firewall.

- Log In to Grafana: You will be prompted to log in. Use the following credentials:
  - Username: admin
  - Password: admin1

You should see a dashboard showing three panels.
If the dashboard is not visible by default, you should find the `rt_linux_tutorial` dashboard under `Provisioned Dashboards` in the Dashboards menu.
