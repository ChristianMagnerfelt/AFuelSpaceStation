#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>
#include <mutex>
#include <iostream>

typedef int64_t Nitrogen;
typedef int64_t QuantumFluid;
	
/*
 *	\brief	A fuel tank which contains a limited amount of nitrogen and quantum fluid
 */
struct FuelTank
{
	FuelTank() : m_maxQ(10), m_maxN(10), m_levelQ(10), m_levelN(10) {}
	FuelTank(QuantumFluid maxQ, Nitrogen maxN, QuantumFluid levelQ, Nitrogen levelN)
		: m_maxQ(maxQ), m_maxN(maxN), m_levelQ(levelQ), m_levelN(levelN) {}
		
	const QuantumFluid m_maxQ;	//!< Maximum capacity of Quantum fluid
	const Nitrogen m_maxN;		//!< Maximum capacity of Nitrogen
	
	QuantumFluid m_levelQ;		//!< Current level of Quantum fluid
	Nitrogen m_levelN;			//!< Current level of Nitrogen
};
/*
 *	\brief	A monitor that provides synchronized output
 */
class SyncOutput
{	
	public:
		SyncOutput(std::ostream & out) : m_out(out) {}
		
		template <typename T>
		void print(const T & value) const
		{
			std::unique_lock<std::mutex> l(m);
			std::cout << value << std::flush;
		}
		
	private:
		std::ostream & m_out;
		mutable std::mutex m;
};
#endif
