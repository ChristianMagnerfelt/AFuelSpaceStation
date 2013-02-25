/*!
 *	\brief	A fuel space station
 *	\author	Christian Magnerfelt
 *	\email	magnerf@kth.se
 */

#include "types.hpp"
#include "space_station.hpp"
#include "space_vehicle.hpp"

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>

const std::size_t MAX_FUEL_VEHICLES = 5;
const std::size_t MAX_VEHICLES = 100;
std::size_t g_numFuelVehicles = MAX_FUEL_VEHICLES;
std::size_t g_numVehicles = MAX_VEHICLES;

SyncOutput g_syncOutput(std::cout);

std::thread g_threads [MAX_FUEL_VEHICLES + MAX_VEHICLES];

int main()
{
	// Create the space station
	SpaceStation alphaStation(FuelTank(2000, 2000, 0, 0), 32);
	
	g_syncOutput.print("Main : Starting threads\n");
	
	std::size_t tIDX = 0;
	// Create fuel vehicle threads
	for(; tIDX < g_numFuelVehicles; ++tIDX)
	{	
		g_threads[tIDX] = std::thread(SpaceFuelVehicle(tIDX, alphaStation, FuelTank(100,100,100,100), FuelTank(30,30,30,30)));
	}
	for(; tIDX < g_numVehicles; ++tIDX)
	{	
		g_threads[tIDX] = std::thread(SpaceVehicle(tIDX, alphaStation, FuelTank(30,30,0,0)));
	}
		
	for(std::size_t i = 0; i < g_numFuelVehicles + g_numVehicles; ++i)
	{
		g_threads[i].join();
	}
	return 0;
}
