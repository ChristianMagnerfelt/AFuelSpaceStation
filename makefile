debug: debug_main.o debug_space_station.o debug_space_vehicle.o
	g++ -o AFuelSpaceStation space_station.o main.o space_vehicle.o -lpthread

debug_space_vehicle.o: space_vehicle.cpp
	g++ -c -g -O0 -Wall -Wextra space_vehicle.cpp -std=c++0x
	
debug_space_station.o: space_station.cpp
	g++ -c -g -O0 -Wall -Wextra space_station.cpp -std=c++0x
	
debug_main.o: main.cpp
	g++ -c -g -O0 -Wall -Wextra main.cpp -std=c++0x


	
clean:
	rm AFuelSpaceStation *.o
