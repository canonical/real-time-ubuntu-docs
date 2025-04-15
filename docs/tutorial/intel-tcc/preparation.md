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

   For the real-time kernel to be used, a reboot is required.
   We will reboot when we [add kernel command-line parameters](kernel-parameters).

   Refer to [How to enable Real-time Ubuntu](https://canonical-ubuntu-pro-client.readthedocs-hosted.com/en/latest/howtoguides/enable_realtime_kernel/) for more details.

## Other required software

Install required tools:
```bash
sudo apt install msr-tools stress-ng mosquitto-clients
```

Download the example code archive [here](tutorial-intel-tcc-code.tar.gz) and extract it.

```bash
tar -xvf tutorial-intel-tcc-code.tar.gz --one-top-level
```

## Statistics infrastructure

This tutorial uses Mosquitto MQTT, Telegraf, InfluxDB and Grafana to capture and visualize the statistics obtained from the test application.
For this tutorial we will run these tools as Docker containers.

### Install Docker and Docker Compose

```{note}
Docker may also be installed by following the [official instructions](https://docs.docker.com/engine/install/ubuntu/).
```

Install Docker and Compose as a snap by running:

```
sudo snap install docker
```

Create the docker group and add your user to it.

```
sudo addgroup --system docker
sudo adduser $USER docker
```

Apply the group changes to your current session:

```
newgrp docker
```

Restart the docker service for it to be aware of the new group:

```
sudo snap disable docker
sudo snap enable docker
```

### Start the services

The docker-compose definition of the statistics infrastructure is provided in the example code archive you downloaded earlier.
Navigate into the `docker-compose` subdirectory, and then execute docker compose:

```bash
cd tutorial-intel-tcc-code/docker-compose
docker compose up -d 
```

Verify that the Grafana, InfluxDB, Mosquitto and Telegraf containers are up and running.
Make sure it says "Up" in the status column.

```
docker compose ps
```

```{note}
It may take a few minutes until Grafana starts up for the first time.
If the web interface is not immediately available, wait a minute and try again.
```

When everything is ready, you can access {spellexception}`Grafana's` web interface by following these steps:
1. Open your preferred web browser.
1. Enter the following URL in the address bar: [http://localhost:3000/](http://localhost:3000/)
   - If you are running Docker on a remote server, replace `localhost` with the server's IP address or domain name.
     Also make sure port 3000 is exposed through your firewall.

1. Log In to Grafana: You will be prompted to log in. Use the following credentials:
   - Username: admin
   - Password: admin1

Browse to the statistics dashboard by going to {menuselection}`Dashboards --> Provisioned Dashboards --> rt_linux_tutorial`.
The dashboard has three panels which will show "No data" at this stage.
