#include "space_station.hpp"

void SpaceStation::refuel(Nitrogen unitsN)
{
	if(unitsN > 0)
	{
		bool done = false;
		std::unique_lock<std::mutex> l(nLock);
		while(!done)
		{
			if(m_n - unitsN >= 0)
			{
				m_n -= unitsN;
				nFlag = true;
				nCv.notify_all();
				done = true;
			}
			else
			{
				nFlag = false;
				while(!nFlag)
				{
					nCv.wait(l);
				}
			}
		}
	}
}	
void SpaceStation::deliverFuel(Nitrogen unitsN)
{
	if(unitsN > 0)
	{
		bool done = false;
		std::unique_lock<std::mutex> l(nLock);
		while(!done)
		{
			if(m_n + unitsN <= 0)
			{
				m_n += unitsN;
				nFlag = true;
				nCv.notify_all();
				done = true;
			}
			else
			{
				nFlag = false;
				while(!nFlag)
				{
					nCv.wait(l);
				}
			}
		}
	}
}
