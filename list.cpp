#include "icecream.h"

#include <string.h>
#include <strings.h>
#include <sstream>
using namespace YAML;

static const char* bukkiturl[] = BUKKITURL;
static const char* metaurl[] = METAURL;

struct BukkitInfo bukkitversion() {
	vector<BukkitInfo> bukkits;
	vector<char*> options;
	int r;

	status("Fetching list of Bukkit versions...");

	char* versiontext = NULL;
	for (int x = 0; x < BUKKITURLS && (!versiontext || !versiontext[0]); x++) {
		versiontext = fetchurl(bukkiturl[x]);
	}
	if (!versiontext || !versiontext[0]) die("Could not fetch list of Bukkit versions.\n");


	istringstream in(versiontext);
	Parser parser(in);
		
	Node doc;
	try {
		while (parser.GetNextDocument(doc)) {
			const Node& node = doc["bukkits"];
			for (unsigned int j = 0; j < node.size(); j++) {
				BukkitInfo bukkitinfo;
				node[j]["version"] >> bukkitinfo.version;
				node[j]["name"] >> bukkitinfo.name;
				for (unsigned int x = 0; x < node[j]["url"].size(); x++) {
					char* url;
					node[j]["url"][x] >> url;
					bukkitinfo.url.push_back(url);
				}
				bukkits.push_back(bukkitinfo);
				options.push_back(strdup(bukkitinfo.name));
			}
		}
	} catch (ParserException e) {
		die("%s\n", e.what());
	}
	
	r = showmenu("Select Bukkit Version", options);
	vecfree(options);
	
	return bukkits[r];
}

void modlist(vector<Mod>* mods, const char* bukkitcode) {
	char* url = NULL;
	unsigned int urllen = 0;
	char* metatext = NULL;
	
	status("Fetching metalist...");

	for (int x = 0; x < METAURLS && (!metatext || !metatext[0]); x++) {
		if (strlen(metaurl[x]) + strlen(bukkitcode) + 6 > urllen) {
			url = (char*)realloc(url, strlen(metaurl[x]) + strlen(bukkitcode) + 6);
			urllen = strlen(metaurl[x]) + strlen(bukkitcode) + 6;
		}
		sprintf(url, "%s-%s.yml", metaurl[x], bukkitcode);
	
		metatext = fetchurl(url);
	}
	if (!metatext || !metatext[0]) die("Could not fetch metalist.\n");
	//printf("metatext='%s'\n", metatext);

	vector<ModList>	lists;
	{
		istringstream in(metatext);
		Parser parser(in);
		
		Node doc;
		try {
			while (parser.GetNextDocument(doc)) {
				const Node& node = doc["lists"];
				for (unsigned int j = 0; j < node.size(); j++) {
					ModList list;
					node[j]["name"] >> list.name;
					for (unsigned int x = 0; x < node[j]["url"].size(); x++) {
						char* url;
						node[j]["url"][x] >> url;
						list.url.push_back(url);
					}
					lists.push_back(list);
				}
			}
		} catch (ParserException e) {
			die("%s\n", e.what());
		}
	}

	list<Mod> modlist;
	for (unsigned int i = 0; i < lists.size(); i++) {
		status("Fetching list: %s", lists[i].name);
		char* listtext = NULL;
		for (unsigned int u = 0; u < lists[i].url.size() && !listtext; u++) {
			listtext = fetchurl(lists[i].url[u]);
		}
		if (!listtext || !listtext[0]) { log("Could not fetch list: %s\n", lists[i].name); continue; }

		istringstream in(listtext);
		Parser parser(in);
		
		Node doc;
		try {
			while (parser.GetNextDocument(doc)) {
				if (doc.FindValue("mods")) {
					const Node& modnode = doc["mods"];
					for (unsigned int j = 0; j < modnode.size(); j++) {
						modlist.push_back(Mod(doc, modnode[j]));
					}
				}
				if (doc.FindValue(bukkitcode)) {
					const Node& modnode = doc[bukkitcode];
					for (unsigned int j = 0; j < modnode.size(); j++) {
						modlist.push_back(Mod(doc, modnode[j]));
					}
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
		
		processdeps(mods, &modlist, mod);
		
		mods->push_back(mod);
	}
}

void processdeps(vector<Mod>* mods, list<Mod>* modlist, const Mod& mod) {
	for (unsigned int i = 0; i < mod.depends.size(); i++) {
		const char* dep = mod.depends[i];
		
		int found = 0;
		for (unsigned int j = 0; j < mods->size(); j++) {
			if ((*mods)[j] == dep) {
				found = 1;
				break;
			}
		}
		if (found) continue;
		
		found = 0;
		for (list<Mod>::iterator j = modlist->begin(); j != modlist->end(); ++j) {
			if (*j == dep) {
				Mod depmod = *j;
				modlist->remove(depmod);
				processdeps(mods, modlist, depmod);
				mods->push_back(depmod);
				log("Added as dependency: %s\n", depmod.name);
				found = 1;
				break;
			}
		}
		if (found == 0) log("Warning: Unsatisfied dependency '%s'\n", dep);
	}
}



// EOF

