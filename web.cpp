#include "icecream.h"

#include <errno.h>
#include <curl/curl.h>

struct FetchInfo {
	char** string;
	int length;
	FILE* fp;
};

size_t fetchurlcb(char *ptr, size_t size, size_t nmemb, void *userdata) {
	struct FetchInfo* fi = (struct FetchInfo*)userdata;
	if (fi->fp) {
		return fwrite(ptr, size, nmemb, fi->fp);
	} else {
		int newlen = fi->length + (size * nmemb);
		*(fi->string) = (char*)realloc(*(fi->string), newlen+1);
		memcpy(*(fi->string)+fi->length, ptr, size*nmemb);
		(*(fi->string))[newlen] = 0;
		fi->length = newlen;
		return size * nmemb;
	}
}

char* fetchurl(const char* url, const char* file) {
	static CURL* curl;
	char* ret = NULL;
	struct FetchInfo fi;
	int rc;

	if (!curl) {
		curl = curl_easy_init();
		if (!curl) die("cURL init failed.");
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "IceCream/0.1 (http://icecream.maeyanie.com)");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &fetchurlcb);
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	}
	
	if (file) {
		fi.string = NULL;
		fi.fp = fopen(file, "wb");
		if (!fi.fp) die("Could not open file '%s' for writing: %s\n", file, strerror(errno));
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
	} else {
		fi.string = &ret;
		fi.length = 0;
		fi.fp = NULL;
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fi);
	rc = curl_easy_perform(curl);
	if (rc) die("Could not fetch URL '%s' to file '%s'\n", url, file);
	
	if (file) {
		fclose(fi.fp);
		return NULL;
	} else {
		return ret;
	}
}



// EOF

