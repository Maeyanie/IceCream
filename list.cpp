#include "icecream.h"

#include <string.h>
#include <strings.h>
#include <sstream>
using namespace YAML;

struct BukkitInfo* bukkitversion() {
	vector<char*> urls;
	vector<char*> codes;
	vector<char*> options;
	int r;

	status("Fetching list of Bukkit versions...");

	char* versiontext = fetchurl(BUKKITURL);
	if (!versiontext || !versiontext[0]) die("Could not fetch list of Bukkit versions.\n");
	
	char* pos;
	char* line = strtok_r(versiontext, "\r\n", &pos);
	char* url, * code, * option;
	do {
		url = strtok(line, " ");
		code = strtok(NULL, " ");
		option = strtok(NULL, "\r\n");
		
		//printf("url=%s\ncode=%s\noption=%s\n", url, code, option);
		
		urls.push_back(url);
		codes.push_back(code);
		options.push_back(option);
	} while ((line = strtok_r(NULL, "\r\n", &pos)));
	
	r = showmenu(options);
	
	BukkitInfo* ret = new BukkitInfo();
	ret->url = strdup(urls[r]);
	ret->code = strdup(codes[r]);
	ret->option = strdup(options[r]);
	return ret;
}

void modlist(vector<Mod>* mods, const char* bukkitcode) {
	char* metaurl = (char*)malloc(strlen(METAURL) + strlen(bukkitcode) + 1);
	sprintf(metaurl, "%s%s", METAURL, bukkitcode);
	
	status("Fetching metalist...");
	char* metatext = fetchurl(metaurl);
	//printf("metatext='%s'\n", metatext);
	
	vector<char*> urls;
	vector<char*> names;

	char* pos;
	char* line = strtok_r(metatext, "\r\n", &pos);
	char* url, * name;
	do {

		//printf("line=%s\n", line);
		
		url = strtok(line, " ");
		name = strtok(NULL, "\r\n");
		
		//printf("url=%s\nname=%s\n", url, name);
		
		urls.push_back(url);
		names.push_back(name);
	} while ((line = strtok_r(NULL, "\r\n", &pos)));
	

	list<Mod> modlist;
	
	for (unsigned int i = 0; i < urls.size(); i++) {
		status("Fetching list: %s", names[i]);
		char* listtext = fetchurl(urls[i]);

		istringstream in(listtext);
		Parser parser(in);
		
		Node doc;
		try {
			while (parser.GetNextDocument(doc)) {
				const Node& modnode = doc["mods"];
				for (unsigned int j = 0; j < modnode.size(); j++) {
					modlist.push_back(Mod(modnode[j]));
				}
			}
		} catch (ParserException e) {
			die("%s\n", e.what());
		}
	}
	
	
	modlist.sort();
	
	int m;
	while ((m = showmenu(modlist)) >= 0) {
		list<Mod>::iterator i = modlist.begin();
		for (int j = 0; j < m; j++) i++;
		Mod mod = *i;
		modlist.remove(mod);
		mods->push_back(mod);
	}
}



// EOF

