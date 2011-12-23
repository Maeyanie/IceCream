#include "icecream.h"

void status(const char* fmt, ...) {
	// TODO
	va_list vl;
	va_start(vl, fmt);
	vprintf(fmt, vl);
	printf("\n");
	fflush(stdout);
	va_end(vl);
}

void log(const char* fmt, ...) {
	// TODO
	va_list vl;
	va_start(vl, fmt);
	vprintf(fmt, vl);
	fflush(stdout);
	va_end(vl);
}

int showmenu(vector<char*>& options) {
	int ret;
	status("");
	// TODO: Fancy menu goes here.
	for (unsigned int x = 0; x < options.size(); x++) {
		printf("%d: %s\n", x+1, options[x]);
	}
	fflush(stdout);
	scanf("%d", &ret);
	return ret-1;
}

int showmenu(list<Mod>& options) {
	int ret;
	int x = 1;
	status("");
	// TODO: Fancy menu goes here.
	printf("1: Done\n");
	for (list<Mod>::iterator i = options.begin(); i != options.end(); i++) {
		printf("%d: %s\n", ++x, (*i).desc);
	}
	fflush(stdout);
	scanf(" %d", &ret);
	return ret-2;
}



// EOF

