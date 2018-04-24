projet:
	g++ -W -Wall -std=c++14 projet.cpp `sdl-config --cflags --libs` -o projet


clean:
	rm projet