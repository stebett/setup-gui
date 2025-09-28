# setup-gui

A custom graphical user interface written in C++ to manage and monitor a behavioral neuroscience setup in real time.

## Overview

This GUI was developed to automate and supervise experiments coordinated by two Arduino Uno boards. While the Arduinos handle low-level experimental events—movement detection, camera triggering, liquid reward delivery, and laser activation—the GUI manages higher-level control and data integrity.

Without the GUI, many critical steps (e.g., activating the setup, enabling the laser system, managing data storage, selecting reward protocols, and compressing videos) had to be performed manually, increasing the risk of mistakes and data loss.

The GUI integrates these tasks into a single interface, ensuring:

- Real-time reporting of setup state
- Automated activation of systems and protocols
- Safe data handling and storage
- Reduced operator workload and error risk

## Features

- Monitors Arduino output and experimental state
- Automates session start-up and shutdown procedures
- Controls laser activation and droplet positioning protocols
- Manages video compression and data storage
- Provides a simplified workflow for both trained and untrained operators



## Event flow

1. Boot → Load config, discover ports, verify storage & laser readiness.
2. Arm → Transition to ARMED, subscribe to Arduino event stream.
3. Run → On IR_CROSSED
  - Reset a 10 s timer
  - Tell cameras to capture via 400 Hz PWM
  - Trigger pump for 20 µL
  - With p = 0.5, trigger laser
  - Save an atomic event record (time, outcomes, device states)
     
4. Background → Rotate files, queue video compression, update progress.  
5. Stop/Safe → Graceful teardown, write an audit trail, and return to IDLE.

## Why it’s hard to break

**Deterministic state machine** → Fewer “unknown” states; easy to audit.  
**Defensive I/O** → Debounced serial parsing, timeouts, and retries.  
**Atomic logging** → Every action is durably written before the next one fires.  
**Idempotent automation** → Restarting tasks won’t corrupt data or double-trigger devices.  
**Human-proofing** → Pre-flight checks + guarded buttons = low risk for new operators.
