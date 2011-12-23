#include "icecream.h"

int main(int argc, char* argv[]) {
	struct BukkitInfo* binfo;
	vector<Mod> mods;
	char yn;
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
	
	modlist(&mods, binfo->code);
	
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
	
	buildjar(binfo, mods);
	
	status(" ");
	//printf("All done. You can find your modded Bukkit jar at: %s/%s\n", dir, BUKKITJAR);
	printf("All done.\n");
	return 0;
}



void realdie(const char* file, int line, const char* fmt, ...) {
	va_list vl;
	va_start(vl, fmt);
	fprintf(stderr, "Error at %s:%d: ", file, line);
	vfprintf(stderr, fmt, vl);
	fflush(stderr);
	va_end(vl);
	exit(1);
}



// EOF

