#include "space_station.hpp"

#include <iostream>
#include <sstream>

extern SyncOutput g_syncOutput;
/*
 *	\brief	Refuels a space vehicle with fuel
 *	\fairness 	se dock() for details
 */
void SpaceStation::refuel(std::size_t thisId, FuelTank & fuelTank)
{	
	bool doneN = false;
	bool doneQ = false;
	Nitrogen requestN = 0;
	QuantumFluid requestQ = 0;
	
	// Check what fuel that we need to request
	if(fuelTank.m_levelQ == fuelTank.m_maxQ || fuelTank.m_maxQ == 0)
		doneQ = true;
	if(fuelTank.m_levelN == fuelTank.m_maxN || fuelTank.m_maxN == 0)
		doneN = true;
		
	// Calculate the exact amount of fuel needed to fill the fuel tank completely
	while(!doneQ || !doneN)
	{	
		// Do we need to refill nitrogen?
		if(!doneN)
		{
			
			// Lock station fuel tank for fuel request
			std::unique_lock<std::mutex> l(fuelLock);
			
			// Calculate the amount needed to refuel
			requestN = fuelTank.m_maxN - fuelTank.m_levelN;
			// Refuel if there is fuel left in the station fuel tank 
			if((m_stationFuelTank.m_levelN - requestN) >= 0)
			{
				m_stationFuelTank.m_levelN = m_stationFuelTank.m_levelN - requestN;
				fuelFlag = true;
				fuelCV.notify_all();
				doneN = true;
			}
			// Implicit release of fuel lock
		}
		
		// Do we need to refill Quantum Fluid?
		if(!doneQ)
		{
			// Lock station fuel tank for fuel request
			std::unique_lock<std::mutex> l(fuelLock);
			
			// Calculate the amount needed to refuel
			requestQ = fuelTank.m_maxQ - fuelTank.m_levelQ;
			
			// Refuel if there is fuel left in the station fuel tank 
			if((m_stationFuelTank.m_levelQ - requestQ) >= 0)
			{
				m_stationFuelTank.m_levelQ -= requestQ;
				fuelFlag = true;
				fuelCV.notify_all();
				doneQ = true;
			}
			// Implicit release of fuel lock
		}
		
		// If either refueling failed sleep and wait until refilled
		if(!doneN || !doneQ)
		{
			// Relock the fuel lock
			std::unique_lock<std::mutex> l(fuelLock);
			fuelFlag = false;
			while(!fuelFlag)
			{
				fuelCV.wait(l);
			}
		}
	}
	if(requestQ > 0 || requestN > 0)
	{	
		// Sleep until quantum fluid tank is refueled
		std::this_thread::sleep_for(std::chrono::seconds(10));
		
		// Fuel has arrived!
		fuelTank.m_levelQ += requestQ;
		fuelTank.m_levelN += requestN;
		
		std::ostringstream out;
		out << thisId << " refueled with " << requestQ << " amount of Quantum fluid" << std::endl;
		out << thisId << " refueled with " << requestN << " amount of Nitrogen" << std::endl;
		g_syncOutput.print(out.str());
	}
}	
/*!
 *	\brief	Deliver fuel to station using fueltank
 *	\fiarness Se dock() for details
 */
void SpaceStation::deliverFuel(std::size_t thisId, FuelTank & fuelTank)
{
	
	bool doneN = false;
	bool doneQ = false;
	
	// Check what fuel that we are able to deliver
	if(fuelTank.m_maxQ == 0)
		doneQ = true;
	if(fuelTank.m_maxN == 0)
		doneN = true;		
		
	while(!doneQ  || !doneN)
	{
		// Do we need to refill nitrogen?
		if(!doneN)
		{
			
			// Lock station fuel tank for fuel deliver
			std::unique_lock<std::mutex> l(fuelLock);
			
			// Refuel if there is space left
			if((m_stationFuelTank.m_levelN + fuelTank.m_levelN) <= m_stationFuelTank.m_maxN)
			{
				// Add fuel to station
				m_stationFuelTank.m_levelN += fuelTank.m_levelN;
				fuelFlag = true;
				fuelCV.notify_all();
				doneN = true;
				
				std::ostringstream out;
				out << thisId << " delivered " << fuelTank.m_levelN << " amount of Nitrogen" << std::endl;
				out << "Station fuel Nitrogen fuel level is: " << m_stationFuelTank.m_levelN << std::endl;							
				g_syncOutput.print(out.str());
			}
			// Implicit release of fuel lock
		}
		
		// Do we need to refill Quantum Fluid?
		if(!doneQ)
		{
			// Lock station fuel tank for fuel delivery
			std::unique_lock<std::mutex> l(fuelLock);
			
			// Refuel if there is space left 
			if((m_stationFuelTank.m_levelQ + fuelTank.m_levelQ) <= m_stationFuelTank.m_maxQ)
			{
				m_stationFuelTank.m_levelQ += fuelTank.m_levelQ;
				fuelFlag = true;
				fuelCV.notify_all();
				doneQ = true;
				
				std::ostringstream out;
				out << thisId << " delivered " << fuelTank.m_levelQ << " amount of Quantum Fluid" << std::endl;
				out << "Station fuel Quantum Fluid fuel level is: " << m_stationFuelTank.m_levelQ << std::endl;
				g_syncOutput.print(out.str());			
			}
			// Implicit release of fuel lock
		}
		
		// If either refueling failed sleep and wait until refilled
		if(!doneN || !doneQ)
		{
			// Relock the fuel lock
			std::unique_lock<std::mutex> l(fuelLock);
			fuelFlag = false;
			while(!fuelFlag)
			{
				fuelCV.wait(l);
			}
		}
	}
}
/*!
 *	\brief		Docks a space vehicle if there is a space left
 *	\fairness	Implementation dependent as threads aquire lock one at the time.
 *				The order for which the lock are aquired depends on the implementation 
 *				of std::unique_lock<std::mutex> and std::mutex.
 *				quoted from Boost.org:
 *				"Note the the lack of reader-writer priority policies in shared_mutex. 
 *				This is due to an algorithm credited to Alexander Terekhov which lets the 
 *				OS decide which thread is the next to get the lock without caring 
 *				whether a unique lock or shared lock is being sought. 
 *				This results in a complete lack of reader or writer starvation. 
 *				It is simply fair.
 *				As std::mutex orginally was boost::mutex it safe to assume that std::mutex
 *				is as fair as the OS allows it to be.
 */
bool SpaceStation::dock(std::size_t thisId)
{
	std::unique_lock<std::mutex> l(dockLock);
	if(m_numDocks > 0)
	{
		--m_numDocks;
		std::ostringstream out;
		out << thisId << " docked!" << std::endl;
		g_syncOutput.print(out.str());
		return true;
	}
	return false;
}
/*!
 *	\brief	Undocks space vehicle from station
 */
void SpaceStation::undock(std::size_t thisId)
{
	std::unique_lock<std::mutex> l(dockLock);
	std::ostringstream out;
	out << thisId << " undocked!" << std::endl;
	g_syncOutput.print(out.str());	
	++m_numDocks;
}
