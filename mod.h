#ifndef ICECREAM_MOD_H
#define ICECREAM_MOD_H

#include <string.h>
#include <yaml-cpp/yaml.h>

inline void vecdup(vector<char*>& lhs, const vector<char*>& rhs) {
	lhs.resize(rhs.size());
	for (unsigned int i = 0; i < rhs.size(); i++) {
		lhs[i] = strdup(rhs[i]);
	}
}

inline void vecfree(vector<char*>& v) {
	for (unsigned int i = 0; i < v.size(); i++) {
		free(v[i]);
	}
}

inline void operator>>(const YAML::Node& lhs, char*& rhs) {
	string value;
	lhs >> value;
	rhs = strdup(value.c_str());
}

class Mod {
public:
	char* mod;
	char* name;
	char* author;
	char* desc;
	char* filename;
	vector<char*> url;
	vector<char*> depends;
	vector<char*> protect;

	Mod() { mod = name = author = desc = filename = NULL; }
	Mod(const Mod& rhs) {
		mod = strdup(rhs.mod);
		name = strdup(rhs.name);
		author = strdup(rhs.author);
		desc = strdup(rhs.desc);
		filename = strdup(rhs.filename);

		vecdup(url, rhs.url);
		vecdup(depends, rhs.depends);
		vecdup(protect, rhs.protect);
	}
	Mod(const YAML::Node& globnode, const YAML::Node& modnode);

	~Mod() {
		if (mod) free(mod);
		if (name) free(name);
		if (author) free(author);
		if (desc) free(desc);
		if (filename) free(filename);
		
		vecfree(url);
		vecfree(depends);
		vecfree(protect);
	}
	
	inline int isdepend(const Mod* rhs) const {
		for (vector<char*>::const_iterator i = depends.begin(); i != depends.end(); ++i) {
			if (!strcasecmp(rhs->mod, *i)) return 1;
		}
		return 0;
	}
	inline int isprotect(const char* path) const {
		for (vector<char*>::const_iterator i = protect.begin(); i != protect.end(); ++i) {
			if (!strncasecmp(path, *i, strlen(*i))) return 1;
		}
		return 0;
	}
	
	inline int operator==(const Mod& rhs) const {
		return !strcasecmp(mod, rhs.mod);
	}
	inline int operator==(const char* rhs) const {
		return !strcasecmp(mod, rhs);
	}
	inline int operator<(const Mod& rhs) const {
		if (isdepend(&rhs)) return 0;
		if (rhs.isdepend(this)) return 1;
		return strcasecmp(mod, rhs.mod) < 0;
	}
	inline int operator>(const Mod& rhs) const {
		if (isdepend(&rhs)) return 1;
		if (rhs.isdepend(this)) return 0;
		return strcasecmp(mod, rhs.mod) > 0;
	}
};

#endif

