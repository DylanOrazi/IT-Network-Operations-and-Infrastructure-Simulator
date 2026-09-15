IT Network Operations and Infrastructure Simulator

Project Category:
Computer Programming / IT Operations Simulation

Overview:
The IT Network Operations and Infrastructure Simulator is a desktop application built in C using raylib. The project simulates a small IT network environment with routers, switches, servers, firewall devices, live health metrics, alerts, incidents, technician actions, and TXT report exporting.

This project was built to demonstrate programming, troubleshooting logic, IT support workflows, and network operations concepts in one interactive application.

Purpose:
The purpose of this project was to create a realistic IT operations simulator that shows how devices in a network can be monitored, selected, reviewed, and acted on by a technician. Instead of only displaying static data, the simulator updates device health, latency, packet loss, alerts, and incidents in real time.

Main Features:
- Graphical desktop interface built with raylib
- Device table view with network device information
- Animated topology view with connected devices
- Clickable device selection
- Device details panel
- Live device metrics including health, latency, packet loss, CPU, and memory
- Alert feed for monitoring events
- Incident tracking system
- Technician actions such as ping, acknowledge, resolve, restart, and escalate
- Action cooldown and feedback system
- Background technician activity log
- TXT session report export

Technology Used:
- C
- raylib
- Visual Studio
- Windows desktop environment
- TXT file export

Keyboard Controls:
- T: Switch between topology view and device table view
- P: Ping selected device
- D: Diagnose or acknowledge selected device issue
- R: Restart or resolve selected device issue
- E: Escalate selected device issue
- X: Export TXT session report
- ESC: Close the application

Report Export:
The simulator can export a TXT session report containing a summary of device health, alerts, incidents, and technician activity.

Example export file:
reports\network_session_report.txt

Skills Demonstrated:
- C programming
- Modular project structure using header and source files
- Graphical interface development with raylib
- Real-time simulation logic
- Basic network operations concepts
- Incident response workflow design
- File output and report generation
- Debugging and troubleshooting in Visual Studio
- User interface layout planning

Project Structure:
- main.c: Main program loop and screen rendering
- device.c / device.h: Device data and status logic
- connection.c / connection.h: Network connection data
- topology.c / topology.h: Animated topology display
- simulation.c / simulation.h: Live metric simulation
- alert.c / alert.h: Alert feed system
- incident.c / incident.h: Incident tracking system
- technician.c / technician.h: Technician actions
- activity_log.c / activity_log.h: Background activity logging
- report.c / report.h: TXT report export system

Screenshots:
Screenshots are included to show the topology view, device table view, device details panel, alert feed, incident panel, and report export confirmation.

Limitations:
This is a simulation project and does not connect to real network devices. Device metrics, incidents, and alerts are generated within the application for demonstration purposes.
