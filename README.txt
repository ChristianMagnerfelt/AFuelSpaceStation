A Fuel Space Station
--------------------

Author:
Christian Magnerfelt

Email:
magnerf@kth.se

Description:
Simulates a fuel station in space which supplies space vehicles with Nitrogen and 
Quantum Fluid. Vehicles consumes Nitrogen and Quantum fluid which are refueled 
at the space station. The amount of vehicles that can be docked are limited. There 
is also fuel trucks which supplies the station with fuel. A fuel truck have a special
docks and do not need to ask for permission to dock.

Each of every vehicle is simulated by a thread and access to the fuelstation is handled 
by the fuel station class internally as a monitor.

Mutal exclusion is achieved using std mutex and std unique lock.

Compiler:
Requires at least GCC version 4.4.7

Usage:
./AFuelSpaceStation

Build:
make debug for debug build

Fairness:
In general it all comes down to how std mutex is implemented and how it shedueles the next thread waiting for
a lock. Se comments in space_Station.cpp for details.
