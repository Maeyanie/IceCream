#include "icecream.h"

struct Settings settings;

int main(int argc, char* argv[]) {
	struct BukkitInfo binfo;
	vector<Mod> mods;
	
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-v")) {
			settings.verbose = 1;
		}
	}

	uiinit();
	
	/*char* origdir = (char*)malloc(256);
	char* dir;
	int rv;
	
	getcwd(origdir, 256);
	do {
		dir = tmpnam(NULL);
		if (!dir) die("Could not create temporary directory.");
		rv = mkdir(dir, 0700);
	} while (rv);

	chdir(dir);*/

	binfo = bukkitversion();
	
	modlist(&mods, binfo.version);

	if (!confirm(&binfo, mods)) {
		uicleanup();
		return 0;
	}
	
	buildjar(&binfo, mods);
	
	status(" ");
	uicleanup();
	
	for (unsigned int x = 0; x < mods.size(); x++) {
		unlink(mods[x].filename);
	}

	//printf("All done. You can find your modded Bukkit jar at: %s/%s\n", dir, BUKKITJAR);
	printf("All done. The '%s' file in the current directory now contains all selected mods.\n", BUKKITJAR);
	
	#ifdef __WIN32__
	system("pause");
	#endif
	return 0;
}



void realdie(const char* file, int line, const char* fmt, ...) {
	va_list vl;
	
	uicleanup();
	va_start(vl, fmt);
	fprintf(stderr, "Error at %s:%d: ", file, line);
	vfprintf(stderr, fmt, vl);
	fflush(stderr);
	va_end(vl);
	exit(1);
}



// EOF

