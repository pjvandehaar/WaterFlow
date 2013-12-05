#include "State.cxx"

int main() {
	//prepare ncurses
	WINDOW *statbar, *simbox;
	int c; //latest input
	initscr();refresh();noecho();raw();keypad(stdscr,true);
	start_color();init_pair(1, COLOR_BLUE, COLOR_BLACK);init_pair(2, COLOR_GREEN, COLOR_BLACK);
	mousemask(ALL_MOUSE_EVENTS,NULL);
	statbar = newwin(1,COLS,0,0);
	simbox = newwin(LINES-2,COLS,1,0);
	wprintw(statbar,"%d rows, %d columns.  PRESS TAB TO QUIT.", LINES-3, COLS-2);
	wrefresh(statbar);
	//prepare state
	State s((COLS-3)/BOXWIDTH,LINES-3);
	s.draw(simbox);
	//main input loop
	while(1){
		c=getch();
		if (c==9||c==3){ //quit
			break;
		} else if (c == 0xa) {
			s.process();
			s.draw(simbox);
		} else if (c == 0x27) {
			for (unsigned i=0;i<10;i++) s.process();
			s.draw(simbox);
		} else if (c == 0x3b) {
			for (unsigned i=0;i<100;i++) s.process();
			s.draw(simbox);
		} else {
			s.handleInput(c);
			s.writetobar(statbar,c);
			s.draw(simbox);
		}
		refresh();
	}
	endwin();
	return 0;
}

