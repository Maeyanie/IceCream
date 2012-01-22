#ifndef ICECREAM_H
#define ICECREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include <vector>
#include <list>
using namespace std;

#ifdef __WIN32__
#define BUKKITURL { \
	"http://d186ocprvpb7jc.cloudfront.net/client/bukkitlist.yml", \
	"http://icecreambukkit.s3.amazonaws.com/client/bukkitlist.yml", \
	"http://icecream.maeyanie.com/client/bukkitlist.yml", \
	}
#define BUKKITURLS 3
#define METAURL { \
	"http://d186ocprvpb7jc.cloudfront.net/client/metalist", \
	"http://icecreambukkit.s3.amazonaws.com/client/metalist", \
	"http://icecream.maeyanie.com/client/metalist", \
	}
#define METAURLS 3
#else
#define BUKKITURL { \
	"https://d186ocprvpb7jc.cloudfront.net/client/bukkitlist.yml", \
	"https://icecreambukkit.s3.amazonaws.com/client/bukkitlist.yml", \
	"http://icecream.maeyanie.com/client/bukkitlist.yml", \
	}
#define BUKKITURLS 3
#define METAURL { \
	"https://d186ocprvpb7jc.cloudfront.net/client/metalist", \
	"https://icecreambukkit.s3.amazonaws.com/client/metalist", \
	"http://icecream.maeyanie.com/client/metalist", \
	}
#define METAURLS 3
#endif
#define BUKKITJAR "craftbukkit.jar"

#include "mod.h"

class BukkitInfo {
public:
	char* version;
	char* name;
	vector<char*> url;
	
	BukkitInfo() { version = name = NULL; }
	BukkitInfo(const BukkitInfo& rhs) {
		version = strdup(rhs.version);
		name = strdup(rhs.name);
		vecdup(url, rhs.url);
	}
	~BukkitInfo() {
		if (version) free(version);
		if (name) free(name);
		vecfree(url);
	}
	
	const BukkitInfo& operator=(const BukkitInfo& rhs) {
		version = strdup(rhs.version);
		name = strdup(rhs.name);
		vecdup(url, rhs.url);
		return rhs;
	}
};

class ModList {
public:
	char* name;
	vector<char*> url;
	
	ModList() { name = NULL; }
	ModList(const ModList& rhs) {
		name = strdup(rhs.name);
		vecdup(url, rhs.url);
	}
	~ModList() {
		if (name) free(name);
		vecfree(url);
	}
	
	const ModList& operator=(const ModList& rhs) {
		name = strdup(rhs.name);
		vecdup(url, rhs.url);
		return rhs;
	}
};

extern struct Settings {
	unsigned verbose:1;
} settings;




struct BukkitInfo bukkitversion();
void modlist(vector<Mod>* mods, const char* bukkitcode);
void processdeps(vector<Mod>* mods, list<Mod>* modlist, const Mod& mod);
void buildjar(const struct BukkitInfo*, vector<Mod>&);

// ui.cpp
void uiinit();
void uicleanup();
void status(const char* fmt, ...) __attribute__((format(printf, 1, 2)));
void log(const char* fmt, ...) __attribute__((format(printf, 1, 2)));
void pbstart();
void pbupdate(double done);
void pbdone();
int showmenu(const char* title, vector<char*>& options);
int showmenu(list<Mod>& options);
int confirm(struct BukkitInfo* binfo, vector<Mod>& mods);

// web.cpp
char* fetchurl(const char* url);
int fetchurl(const char* url, const char* file);

#define die(a...) realdie(__FILE__, __LINE__, a)
void realdie(const char* file, int line, const char* fmt, ...) __attribute__((format(printf, 3, 4)));

#endif

