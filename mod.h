#ifndef ICECREAM_MOD_H
#define ICECREAM_MOD_H

#include <string.h>
#include <yaml-cpp/yaml.h>

class Mod {
public:
	char* mod;
	char* name;
	char* author;
	char* desc;
	char* filename;
	vector<char*> url;
	vector<char*> depends;

	Mod() { mod = name = author = desc = filename = NULL; }
	Mod(const Mod& rhs) {
		mod = strdup(rhs.mod);
		name = strdup(rhs.name);
		author = strdup(rhs.author);
		desc = strdup(rhs.desc);
		filename = strdup(rhs.filename);

		url.resize(rhs.url.size());
		for (unsigned int i = 0; i < rhs.url.size(); i++) {
			url[i] = strdup(rhs.url[i]);
		}
		
		depends.resize(rhs.depends.size());
		for (unsigned int i = 0; i < rhs.depends.size(); i++) {
			depends[i] = strdup(rhs.depends[i]);
		}
	}
	Mod(const YAML::Node& globnode, const YAML::Node& modnode);

	~Mod() {
		if (mod) free(mod);
		if (name) free(name);
		if (author) free(author);
		if (desc) free(desc);
		if (filename) free(filename);
		for (vector<char*>::iterator i = url.begin(); i != url.end(); ++i) {
			free(*i);
		}
		for (vector<char*>::iterator i = depends.begin(); i != depends.end(); ++i) {
			free(*i);
		}

	}
	
	int isdepend(const Mod* rhs) const {
		for (vector<char*>::const_iterator i = depends.begin(); i != depends.end(); ++i) {
			if (!strcasecmp(rhs->mod, *i)) return 1;
		}
		return 0;
	}
	
	int operator==(const Mod& rhs) const {
		return !strcasecmp(mod, rhs.mod);
	}
	int operator==(const char* rhs) const {
		return !strcasecmp(mod, rhs);
	}
	int operator<(const Mod& rhs) const {
		if (isdepend(&rhs)) return 0;
		if (rhs.isdepend(this)) return 1;
		return strcasecmp(mod, rhs.mod) < 0;
	}
	int operator>(const Mod& rhs) const {
		if (isdepend(&rhs)) return 1;
		if (rhs.isdepend(this)) return 0;
		return strcasecmp(mod, rhs.mod) > 0;
	}
};

#endif

