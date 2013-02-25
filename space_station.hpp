#ifndef SPACE_STATION_HPP
#define SPACE_STATION_HPP

#include "types.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>

/*
 *	\brief	A space station refuels docked space vehicles with fuel
 *			and recieves fuel deliveries.
 */
class SpaceStation
{
	public:
		SpaceStation(FuelTank fuelTank, std::size_t numDocks) :
					m_stationFuelTank(fuelTank), 
					m_numDocks(numDocks){}
	
		void refuel(std::size_t id, FuelTank & fuelTank);	
		void deliverFuel(std::size_t id, FuelTank & fuelTank);
		bool dock(std::size_t id);
		void undock(std::size_t id);
	private:
		FuelTank m_stationFuelTank;	//!< The storage of the fuel inside the space station.
		
		bool fuelFlag;
		std::condition_variable fuelCV;
		std::mutex fuelLock;	//!< Only one vehicle may request fuel at the time.
		
		std::size_t m_numDocks;
		std::mutex dockLock;	//!< Only one vehicle may request permission to dock at one time.
};
#endif
