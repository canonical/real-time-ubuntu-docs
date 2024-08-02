Real-time publish-subscribe over OPC-UA
=======================================

..
    Hardware:
      - Two machines
      - Intel CPU
      - Intel NIC (i225 ok for testing, i226 for tutorial)
      - Ad-hoc Ethernet connection

    Software:
      - A publisher/subscriber application suite based on the open62541 OPC-UA example
      - Need to modify the upstream code
    
    Tutorial plan:
      - Start from Ubuntu 24.04
      - Deploy the publisher/subscriber on different machines
        - Measure and visualize KPIs
      - Switch to RT kernel
        - Measure and visualize KPIs
      - Configure the networking cards for TSN
        - Measure and visualize KPIs
      - Configure the system for TCC (cache partitioning / CAT, GPU isolation)
        - Measure and visualize KPIs

