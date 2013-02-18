#ifndef SPACE_STATION_HPP
#define SPACE_STATION_HPP

#include <mutex>
#include <condition_variable>

class SpaceStation
{
	typedef int64_t Nitrogen;
	typedef int64_t QuantumFluid;
	public:
		void refuel(Nitrogen unitsN);	
		void deliverFuel(Nitrogen unitsN);
	private:
		
		Nitrogen m_n;
		QuantumFluid m_q;
		bool nFlag;
		bool qFlag;
		std::condition_variable nCv;
		std::condition_variable qCv;
		std::mutex nLock;
		std::mutex qLock;
};
#endif
