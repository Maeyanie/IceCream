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
	"http://d186ocprvpb7jc.cloudfront.net/client/bukkitlist.txt", \
	"http://icecreambukkit.s3.amazonaws.com/client/bukkitlist.txt" \
	"http://icecream.maeyanie.com/client/bukkitlist.txt", \
	}
#define BUKKITURLS 3
#define METAURL { \
	"http://d186ocprvpb7jc.cloudfront.net/client/metalist", \
	"http://icecreambukkit.s3.amazonaws.com/client/metalist" \
	"http://icecream.maeyanie.com/client/metalist", \
	}
#define METAURLS 3
#else
#define BUKKITURL { \
	"https://d186ocprvpb7jc.cloudfront.net/client/bukkitlist.txt", \
	"https://icecreambukkit.s3.amazonaws.com/client/bukkitlist.txt" \
	"http://icecream.maeyanie.com/client/bukkitlist.txt", \
	}
#define BUKKITURLS 3
#define METAURL { \
	"https://d186ocprvpb7jc.cloudfront.net/client/metalist", \
	"https://icecreambukkit.s3.amazonaws.com/client/metalist" \
	"http://icecream.maeyanie.com/client/metalist", \
	}
#define METAURLS 3
#endif
#define BUKKITJAR "craftbukkit.jar"

#include "mod.h"

struct BukkitInfo {
	char* url;
	char* code;
	char* option;
};



struct BukkitInfo* bukkitversion();
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

