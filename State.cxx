//a State contains a big 2d array. It doesn't contain the windowing mechanisms, though it draws to ncurses.

#include "State.h"

State::State(unsigned a,unsigned b){
  wd=a;ht=b;
  size=wd*ht;
  water=new double[size];
  land=new unsigned[size];
  dry=new bool[size];
  for (unsigned i=0;i<size;i++) {water[i]=0;land[i]=0;}
  deltaLand=10;
}

void State::draw(WINDOW* win) {
  wmove(win,0,0);
  for (unsigned r=0;r<ht;r++){
    wmove(win,r,0);
    for (unsigned c=0;c<wd;c++){
      unsigned index = c+r*wd;
      wattrset(win,COLOR_PAIR(1));
      wprintw(win," %2.0f",water[index]);
      wattrset(win,COLOR_PAIR(2));
      wprintw(win,"%2.0d",land[index]);
    }
    waddch(win,'\n');
  }
  wrefresh(win);
}

void State::handleInput(int c) {
  if (c == KEY_MOUSE) {
    MEVENT event;
    if (getmouse (&event) == OK) {
      unsigned index = (event.x/BOXWIDTH)+wd*(event.y-1);
      land[index]+=deltaLand;
    }
  } else if (c == '`') {
    deltaLand = -deltaLand;
  } else if (c>='1' && c<='9') {
    deltaLand = 10*(c-'0');
  }
}

void State::writetobar(WINDOW* win, int a) {
  wmove(win,0,0);
  wprintw(win,"%4x",a);
  waddstr(win,"  deltaLand is ");
  wprintw(win,"%+d",deltaLand);
  waddch(win,'\n');
  wrefresh(win);
}

void State::process() {
  double* WATER= new double[size]; //the new map
  for (unsigned i=0;i<size;i++){
    dry[i]=(water[i]<=land[i]);
  }
  for (unsigned y=0;y<ht;y++){
    for (unsigned x=0;x<wd;x++){
      unsigned index=x+y*wd;
      if (!dry[index]) {
	unsigned numNeighbors=1;
	double sumNeighbors=water[index];
	vector<unsigned> toCheck;
	if (y>0)   toCheck.push_back(index-wd);
	if (x>0)   toCheck.push_back(index-1 );
	if (y+1<ht)toCheck.push_back(index+wd);
	if (x+1<wd)toCheck.push_back(index+1 );
	for (vector<unsigned>::iterator i=toCheck.begin();i!=toCheck.end();i++) if (land[*i]<water[index]) {
	    sumNeighbors+=water[*i];numNeighbors++;
	  }
	WATER[index]=sumNeighbors/numNeighbors;
      } else { //dry
	double biggest = 0;
	vector<unsigned> toCheck;
	if (y>0)   toCheck.push_back(index-wd);
	if (x>0)   toCheck.push_back(index-1 );
	if (y+1<ht)toCheck.push_back(index+wd);
	if (x+1<wd)toCheck.push_back(index+1 );
	for (vector<unsigned>::iterator i=toCheck.begin();i!=toCheck.end();i++) if (!dry[*i]) {
	    if (water[*i] > biggest) biggest=water[*i];
	  }
	if (biggest >= land[index]) WATER[index]=(biggest+land[index])/2;
      }
    }
  }
  water = WATER;
  water[0]=99;
  water[size-1]=0;
}

