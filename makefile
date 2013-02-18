debug: a_fuel_space_station.cpp
	g++ -g -O0 -Wall -Wextra -o AFuelSpaceStation a_fuel_space_station.cpp -lpthread -std=c++0x
	
release: a_fuel_space_station.cpp
	g++ -O3 -Wall -Wextra -o AFuelSpaceStation a_fuel_space_station.cpp -lpthread -std=c++0x
	
clean:
	rm AFuelSpaceStation
