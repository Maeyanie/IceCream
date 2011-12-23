#ifndef ICECREAM_H
#define ICECREAM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include <vector>
#include <list>
using namespace std;

#define BUKKITURL "http://icecream.maeyanie.com/client/bukkitlist.php"
#define METAURL "http://icecream.maeyanie.com/client/metalist.php?code="
#define BUKKITJAR "craftbukkit.jar"

#include "mod.h"

struct BukkitInfo {
	char* url;
	char* code;
	char* option;
};

struct FileDownload {
	const char* name;
	const char* url;
	const char* filename;
	
	FileDownload(const char* n, const char* u, const char* f) : name(n), url(u), filename(f) {};
};


struct BukkitInfo* bukkitversion();
void modlist(vector<Mod>* mods, const char* bukkitcode);
void processdeps(vector<Mod>* mods, list<Mod>* modlist, const Mod& mod);
void buildjar(const struct BukkitInfo*, vector<Mod>&);

// ui.cpp
void status(const char* fmt, ...);
void log(const char* fmt, ...);
int showmenu(vector<char*>& options);
int showmenu(list<Mod>& options);

// web.cpp
char* fetchurl(const char* url);
void fetchurl(const char* url, const char* file);
void fetchfiles(vector<FileDownload>& files);

#define die(a...) realdie(__FILE__, __LINE__, a)
void realdie(const char* file, int line, const char* fmt, ...) __attribute__((format(printf, 3, 4)));

#endif

