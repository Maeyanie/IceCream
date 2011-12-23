#include "icecream.h"
#include <ncurses.h>
#include <panel.h>

static WINDOW* wmain;
static PANEL* pmain;
static WINDOW* wstatus;
static PANEL* pstatus;

void uiinit() {
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	
	wstatus = newwin(1, COLS, LINES-1, 0);
	pstatus = new_panel(wstatus);

	wmain = newwin(LINES-1, COLS, 0, 0);
	pmain = new_panel(wmain);
	
	update_panels();
	doupdate();
}

void uicleanup() {
	delwin(wmain);
	delwin(wstatus);
	endwin();
}

void status(const char* fmt, ...) {
	va_list vl;
	va_start(vl, fmt);
	wmove(wstatus, 0, 0);
	wprintw(wstatus, "\n");
	wmove(wstatus, 0, 0);
	vwprintw(wstatus, fmt, vl);
	update_panels();
	doupdate();
	va_end(vl);
}

void log(const char* fmt, ...) {
	va_list vl;
	va_start(vl, fmt);
	vwprintw(wmain, fmt, vl);
	update_panels();
	doupdate();
	va_end(vl);
}

void pbupdate(double done) {
	int r, c;
	char fmt[16];
	char bar[COLS-1];

	getyx(wmain, r, c);

	done *= (COLS-2);
	if (done > COLS-2) done = COLS-2; // Shouldn't be possible, but...
	
	int i;
	for (i = 0; i < done; i++) bar[i] = '=';
	bar[i] = 0;
	
	sprintf(fmt, "[%%-%ds]", COLS-2);
	mvwprintw(wmain, r, 0, fmt, bar);
	fflush(stdout);
}

int showmenu(const char* title, vector<char*>& options) {
	int ch;
	int line = 0;
	int offset = 0;
	WINDOW* menu = newwin(LINES-1, COLS, 0, 0);
	PANEL* pmenu = new_panel(menu);
	status(" ");
	
	wprintw(menu, title);
	
	do {
		int start = offset;
		int end = offset + (LINES-4);
		if (end > (int)options.size()) end = options.size();
		for (int x = start; x < end; x++) {
			mvwprintw(menu, x-start+3, 0, "%d: %s\n", x+1, options[x]);
			if (x == line) mvwchgat(menu, x-start+3, 0, -1, A_REVERSE, 0, NULL);
		}
		update_panels();
		doupdate();
		
		ch = getch();
		switch (ch) {
		case KEY_UP:
			line--;
			if (line < 0) line = 0;
			if (line-1 <= start) {
				offset--;
				if (offset < 0) offset = 0;
			}
			break;
		case KEY_DOWN:
			line++;
			if (line+start+4 >= LINES) offset++;
			break;
		}
	} while (ch != '\n');
	
	del_panel(pmenu);
	delwin(menu);
	refresh();
	return line;
}

int showmenu(list<Mod>& options) {
	int ch;
	int line = 0;
	int offset = 0;
	WINDOW* menu = newwin(LINES-1, COLS, 0, 0);
	PANEL* pmenu = new_panel(menu);
	status(" ");
	
	wprintw(menu, "Select Mods");
	
	do {
		int start = offset;
		int end = offset + (LINES-4);
		if (end > (int)options.size()) end = options.size();

		list<Mod>::iterator i = options.begin();
		int x = 0;
		while (x < start) x++, i++;
		if (x == 0) {
			mvwprintw(menu, 3, 0, "Done\n");
			if (line == 0) mvwchgat(menu, 3, 0, -1, A_REVERSE, 0, NULL);
			x++;
		}
		while (x <= end && i != options.end()) {
			mvwprintw(menu, x-start+3, 0, "%d: %s\n", x, (*i).desc);
			if (x == line) mvwchgat(menu, x-start+3, 0, -1, A_REVERSE, 0, NULL);
			x++, i++;
		}
		end = x;
		update_panels();
		doupdate();
		
		ch = getch();
		switch (ch) {
		case KEY_UP:
			line--;
			if (line < 0) line = 0;
			if (line-1 <= start) {
				offset--;
				if (offset < 0) offset = 0;
			}
			break;
		case KEY_DOWN:
			line++;
			if (line > end) line = end;
			if (line+start+4 >= LINES) offset++;
			break;
		}
	} while (ch != '\n');
	
	del_panel(pmenu);
	delwin(menu);
	update_panels();
	doupdate();
	return line-1;
}

int confirm(struct BukkitInfo* binfo, vector<Mod>& mods) {
	char yn;
	
	status(" ");
	wprintw(wmain, "\nAbout to download Bukkit %s, and add the following mods in this order:\n", binfo->code);
	for (unsigned int i = 0; i < mods.size(); i++) {
		wprintw(wmain, "%d: %s\n", i+1, mods[i].name);
	}
	wprintw(wmain, "Are you sure? (y/N): ");
	update_panels();
	doupdate();
	
	yn = getch();
	
	if (tolower(yn) != 'y') {
		wprintw(wmain, "Exiting.\n");
		return 0;
	}
	return 1;
}



// EOF

