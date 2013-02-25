#ifndef SPACE_VEHICLE_HPP
#define SPACE_VEHICLE_HPP

#include "space_station.hpp"
#include "types.hpp"

/*!
 *	\brief	A space vehicle that delivers fuel to the space station
 */		
class SpaceFuelVehicle
{
	public:
		SpaceFuelVehicle(std::size_t id, SpaceStation & startStation, FuelTank storageFuelTank, FuelTank engineFuelTank) : 
			m_id(id),
			m_currentStation(startStation),  
			m_storage(storageFuelTank), 
			m_engine(engineFuelTank){}
		// Thread main loop
		void operator()();
	private:
		std::size_t m_id;
		SpaceStation & m_currentStation;	//!< Current location of the vehicle
		FuelTank m_storage;
		FuelTank m_engine;
};
/*!
 *	\brief	A regular vehicle that only refuel its fuel tank at the space station
 */
class SpaceVehicle
{
	public:
		SpaceVehicle(std::size_t id, SpaceStation & startStation, FuelTank engineFuelTank) : 
			m_id(id),
			m_currentStation(startStation), 
			m_engine(engineFuelTank){}
		// Thread main loop
		void operator()();
	private:
		std::size_t m_id;
		SpaceStation & m_currentStation;	//!< Current location of the vehicle
 		FuelTank m_engine;
};

#endif
