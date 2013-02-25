#include "space_vehicle.hpp"

/*!
 *	\brief			Thread function that simulate a fuel space vehicle
 *	\description	Burn fuel and sleeps while the vehicle is out from the station. Delivers fuel
 *					to the station and refuel its own fuel at the station.
 */
void SpaceFuelVehicle::operator()()
{
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 20));
					
		m_currentStation.deliverFuel(m_id, m_storage);
		m_currentStation.refuel(m_id, m_engine);

		// Burn fuel
		m_engine.m_levelN = 0;
		m_engine.m_levelQ = 0;
	}
}
/*!
 *	\brief			Thread function that simulate a regular space vehicle
 *	\description	Burn fuel and sleeps while the vehicle is out from the station. 
 *					It also refuel its own fuel at the station
 */
void SpaceVehicle::operator()()
{
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 20));
		
		if(m_currentStation.dock(m_id))
		{
			m_currentStation.refuel(m_id, m_engine);
			m_currentStation.undock(m_id);
		}

		// Burn fuel
		m_engine.m_levelN = 0;
		m_engine.m_levelQ = 0;
	}
}
