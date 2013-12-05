//a State contains a big 2d array. It doesn't contain the windowing mechanisms, though it draws to ncurses.

#ifndef STATE_H_
#define STATE_H_

#define BOXWIDTH 5

#include <cstdio>	//?
#include <cstdlib>	//exit(),srand(),rand()
#include <unistd.h>	//usleep()
#include <ncurses.h>
#include <vector>	
#include <algorithm>
using namespace std;

class State {
	public:
		State(unsigned a,unsigned b);
		void draw(WINDOW* win);
		void handleInput(int c);
		void writetobar(WINDOW* win, int a);
		void process();
	private:
		unsigned wd,ht,size;
		double *water;
		unsigned *land;
		bool *dry;
		unsigned deltaLand;
};

#endif

