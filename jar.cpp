#include "icecream.h"

#include <errno.h>
#include <zip.h>

void buildjar(const struct BukkitInfo* binfo, vector<Mod>& mods) {
	list<struct zip*> zips;
	struct zip* bzp;
	struct zip* mzp;
	struct zip_source* source;
	const char* name;
	int err;
	int index;
	int rc;

	status("Downloading files...");
	
	status("Downloading: Bukkit");
	rc = 0;
	for (unsigned int u = 0; u < binfo->url.size() && !rc; u++) {
		rc = fetchurl(binfo->url[u], BUKKITJAR);
	}
	if (!rc) die("Could not download craftbukkit.jar from any URL\n");
	
	for (vector<Mod>::iterator i = mods.begin(); i != mods.end(); ++i) {
		status("Downloading: %s", i->name);
		rc = 0;
		for (unsigned int u = 0; u < i->url.size() && !rc; u++) {
			rc = fetchurl(i->url[u], i->filename);
		}
		if (!rc) die("Could not download mod %s from any URL\n", i->name);
	}
	
	status("Building Bukkit jar: Preparing...");
	bzp = zip_open(BUKKITJAR, ZIP_CHECKCONS, &err);
	if (!bzp) {
		char errbuf[256];
		zip_error_to_str(errbuf, 256, err, errno);
		die("Could not open '%s' for writing: %s\n", BUKKITJAR, errbuf);
	}
	
	for (vector<Mod>::iterator i = mods.begin(); i != mods.end(); ++i) {
		status("Building Bukkit jar: %s", (*i).name);

		mzp = zip_open((*i).filename, ZIP_CHECKCONS, &err);
		if (!mzp) {
			char errbuf[256];
			zip_error_to_str(errbuf, 256, err, errno);
			die("Could not open '%s' for reading: %s\n", (*i).filename, errbuf);
		}
		zips.push_back(mzp);
		
		int count = zip_get_num_files(mzp); // Not sure how expensive this is...
		for (int x = 0; x < count; x++) {
			name = zip_get_name(mzp, x, 0);
			index = zip_name_locate(bzp, name, ZIP_FL_NOCASE);
			//log(" - %d: %s (%d)\n", x, name, index);
			int protect = 0;
			for (vector<Mod>::iterator j = mods.begin(); j != i; ++j) {
				if (j->isprotect(name)) { protect = 1; break; }
			}
			if (protect) {
				//log("    Skipping due to protection.\n");
				continue;
			}
			
			source = zip_source_zip(bzp, mzp, x, 0, 0, -1);
			if (source == NULL) { die("Could not read %s/%s: %s\n", (*i).filename, name, zip_strerror(bzp)); }
			
			if (index == -1) {
				err = zip_add(bzp, name, source);
				if (err == -1) die("Could not add %s: %s\n", name, zip_strerror(bzp));
			} else {
				err = zip_replace(bzp, index, source);
				if (err == -1) die("Could not replace %s: %s\n", name, zip_strerror(bzp));
			}
		}
		// If we close this here, zip_close() at the end crashes.
		//zip_close(mzp);
	}
	status("Building Bukkit jar: Done!");
	
	zip_close(bzp);

	for (list<struct zip*>::iterator i = zips.begin(); i != zips.end(); i++) {
		zip_close(*i);
	}
}

