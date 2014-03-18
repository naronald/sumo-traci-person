sumo-traci-person
=================

Patch for SUMO to interface with Persons via TraCI. (Not really a patch, but
complete files that a patch can be created from.)

SUMO (sumo-sim.org) is an open-source traffic microsimulation tool.

A command interface, known as TraCI, has been developed to interact with
simulations as they are running. The scripts can retrieve information from the
simulation (e.g., time step, location of a vehicle) and also update information
(e.g., the destination of a vehicle).

At this point, interacting with persons is not included. This is a first attempt
at incorporating Person objects into the TraCI implementation. Some tests are
also included.

This release (based on SUMO 0.20.0) includes the following commands:
* id list (0x00): returns a list of ids of all persons currently running 
within the scenario
* count (0x01): returns the number of persons currently running 
within the scenario
* road id (0x50): returns the id of the edge the named person was at within 
the last step
* stage (0x69): returns the current stage of the named person (walking, waiting,
driving)

VAR\_STAGE is a new command added to TraCIConstants.h.

Future work:
* updating the TraCI python interface
* including more commands
