#include "icecream.h"
using namespace YAML;

Mod::Mod(const YAML::Node& globnode, const YAML::Node& modnode) {
	const Node* key;
	char* value;
	
	if (modnode.FindValue("mod")) {
		modnode["mod"] >> mod;
		modnode["name"] >> name;
		modnode["desc"] >> desc;
	} else { // Backwards compatible mode for <= 0.3
		modnode["name"] >> mod;
		modnode["desc"] >> name;
		desc = strdup("");
	}
	
	switch (modnode["url"].Type()) {
	case NodeType::Scalar:
		modnode["url"] >> value;
		url.push_back(value);
		break;
	case NodeType::Sequence:
		for (YAML::Iterator i = modnode["url"].begin(); i != modnode["url"].end(); ++i) {
			*i >> value;
			url.push_back(value);
		}
		break;
	default:
		die("url: node was not Scalar or Sequence.\n");	
	}
	
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
		filename = strdup(strrchr(url[0], '/') + 1);
	}
	
	if ((key = modnode.FindValue("depends"))) {
		for (YAML::Iterator i = key->begin(); i != key->end(); ++i) {
			(*i) >> value;
			depends.push_back(value);
		}
	}

	if ((key = modnode.FindValue("protect"))) {
		for (YAML::Iterator i = key->begin(); i != key->end(); ++i) {
			(*i) >> value;
			protect.push_back(value);
		}
	}
}



// EOF

