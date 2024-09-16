Step-by-Step guide to real-time GPIO control
============================================

..
    Hardware:
      - A machine with an available GPIO chip
      - Intel CPU

    Software:
      - An application that measures the time difference between when a thread is scheduled to wake up and when it actually wakes up.
    
    All the source code should be placed inside: ./gpio-control
    
    ToC:
      - Intro
        - Start from Ubuntu 24.04 with the generic kernel
        - Measure KPIs
      - Real-time tuning
        - Switch to RT kernel
        - BIOS and kernel command line configuration
        - Measure KPIs
      - Cache reservation
        - Stress the system with additional workload
        - Measure KPIs
      - Boost single threaded performance
        - Use Intel Speed Shift for Edge Computing
        - Measure KPIs
