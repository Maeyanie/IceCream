#include "icecream.h"
#include <curses.h>
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
	vwprintw(wstatus, fmt, vl);
	wprintw(wstatus, "\n");
	va_end(vl);

	va_start(vl, fmt);
	vwprintw(wmain, fmt, vl);
	wprintw(wmain, "\n");
	va_end(vl);

	update_panels();
	doupdate();
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
	char bar[COLS-2];

	getyx(wmain, r, c);

	done *= (COLS-3);
	if (done > COLS-3) done = COLS-3; // Shouldn't be possible, but...
	
	int i;
	for (i = 0; i < done; i++) bar[i] = '=';
	bar[i] = 0;
	
	sprintf(fmt, "[%%-%ds]", COLS-3);
	mvwprintw(wmain, r, 0, fmt, bar);
	
	update_panels();
	doupdate();
}

void showinfo(WINDOW* info, const Mod& mod) {
	int r = 0;
	int w = 0;
	char* text;
	char* word;
	
	werase(info);
	
	mvwprintw(info, r++, 0, "Mod: %s\n", mod.name);
	mvwprintw(info, r++, 0, "\n");
	mvwprintw(info, r++, 0, "Author: %s\n", mod.author);
	mvwprintw(info, r++, 0, "\n");
	mvwprintw(info, r++, 0, "URL: %s\n", mod.url);
	mvwprintw(info, r++, 0, "\n");
	mvwprintw(info, r++, 0, "Filename: %s\n", mod.filename);
	mvwprintw(info, r++, 0, "\n");
	
	mvwprintw(info, r++, 0, "Description:\n");
	text = strdup(mod.desc);
	word = strtok(text, " ");
	while (word) {
		if (w + strlen(word) + 1 >= getmaxx(info)) {
			waddch(info, '\n');
			wmove(info, r++, 0);
			waddstr(info, word);
			w = strlen(word);
		} else {
			waddch(info, ' ');
			waddstr(info, word);
			w += strlen(word) + 1;
		}
		word = strtok(NULL, " ");
	}
	free(text);
	waddch(info, '\n');
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
			if (line >= end) line = end-1;
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
	WINDOW* menu = newwin(LINES-1, COLS/2, 0, 0);
	PANEL* pmenu = new_panel(menu);
	WINDOW* info = newwin(LINES-1, COLS+1/2, 0, COLS/2);
	PANEL* pinfo = new_panel(info);
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
			if (x == line) {
				mvwchgat(menu, x-start+3, 0, -1, A_REVERSE, 0, NULL);
				showinfo(info, *i);
			}
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
			if (line >= end) line = end-1;
			if (line+start+4 >= LINES) offset++;
			break;
		}
	} while (ch != '\n');
	
	del_panel(pinfo);
	delwin(info);
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

