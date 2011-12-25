#include "icecream.h"
using namespace YAML;

void operator>>(const Node& lhs, char*& rhs) {
	string value;
	lhs >> value;
	rhs = strdup(value.c_str());
}

Mod::Mod(const YAML::Node& globnode, const YAML::Node& modnode) {
	const Node* key;
	char* value;
	
	modnode["name"] >> name;
	modnode["desc"] >> desc;
	modnode["url"] >> url;
	
	if ((key = modnode.FindValue("author"))) {
		*key >> author;
	} else if ((key = globnode.FindValue("author"))) {
		*key >> author;
	} else {
		author = strdup("???");
	}
	
	if ((key = modnode.FindValue("filename"))) {
		*key >> filename;
	} else {
		filename = strdup(strrchr(url, '/') + 1);
	}
	
	if ((key = modnode.FindValue("depends"))) {
		for (YAML::Iterator i = key->begin(); i != key->end(); ++i) {
			(*i) >> value;
			depends.push_back(value);
		}
	}
}



// EOF

