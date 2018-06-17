output: 
	g++ -std=c++11 ./src/enhanced_armsimulator.cpp ./src/main.cpp -o processor_simulator 
clean: 
	rm -r processor_simulator