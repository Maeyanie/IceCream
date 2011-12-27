#include "icecream.h"

void uiinit() {}

void uicleanup() {}

void status(const char* fmt, ...) {
	va_list vl;
	va_start(vl, fmt);
	vprintf(fmt, vl);
	printf("\n");
	fflush(stdout);
	va_end(vl);
}

void log(const char* fmt, ...) {
	va_list vl;
	va_start(vl, fmt);
	vprintf(fmt, vl);
	fflush(stdout);
	va_end(vl);
}

void pbstart() {}

void pbupdate(double done) {
	char bar[51];

	done *= 50.0;
	if (done > 50.0) done = 50.0; // Shouldn't be possible, but...
	
	int i;
	for (i = 0; i < done; i++) bar[i] = '=';
	bar[i] = 0;
	
	printf("\r[%-50s]", bar);
	fflush(stdout);
}

void pbdone() {}

int showmenu(const char* title, vector<char*>& options) {
	int ret;
	status(" ");

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
	status(" ");

	printf("1: Done\n");
	for (list<Mod>::iterator i = options.begin(); i != options.end(); i++) {
		printf("%d: %s\n", ++x, (*i).desc);
	}
	fflush(stdout);
	scanf(" %d", &ret);
	return ret-2;
}

int confirm(struct BukkitInfo* binfo, vector<Mod>& mods) {
	char yn;
	
	status(" ");
	printf("About to download Bukkit %s, and add the following mods in this order:\n", binfo->code);
	for (unsigned int i = 0; i < mods.size(); i++) {
		printf("%d: %s\n", i+1, mods[i].name);
	}
	printf("Are you sure? (y/N): ");
	fflush(stdout);
	scanf(" %c", &yn);
	if (tolower(yn) != 'y') {
		printf("Exiting.\n");
		return 0;
	}
	return 1;
}



// EOF

