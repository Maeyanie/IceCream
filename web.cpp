#include "icecream.h"

#include <errno.h>
#include <curl/curl.h>

struct FetchInfo {
	char** string;
	int length;
};

static CURL* curl;

size_t fetchurlstr(char *ptr, size_t size, size_t nmemb, void *userdata) {
	struct FetchInfo* fi = (struct FetchInfo*)userdata;

	int newlen = fi->length + (size * nmemb);

	*(fi->string) = (char*)realloc(*(fi->string), newlen+1);

	memcpy(*(fi->string)+fi->length, ptr, size*nmemb);
	(*(fi->string))[newlen] = 0;

	fi->length = newlen;
	return size * nmemb;
}

size_t fetchurlfile(char *ptr, size_t size, size_t nmemb, void *userdata) {
	return fwrite(ptr, size, nmemb, (FILE*)userdata);
}

int progresscb(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
	pbupdate(dlnow / dltotal);
	return 0;
}

void curlsetup() {
	curl = curl_easy_init();
	if (!curl) die("cURL init failed.");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "IceCream/0.3 (http://icecream.maeyanie.com)");
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, &progresscb);
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
}

char* fetchurl(const char* url) {
	char* ret = NULL;
	struct FetchInfo fi;
	long rc;

	if (!curl) curlsetup();
	
	fi.string = &ret;
	fi.length = 0;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &fetchurlstr);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fi);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	rc = curl_easy_perform(curl);
	if (rc) die("Could not fetch URL '%s'\n", url);

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &rc);
	if (rc/100 > 3) die("Could not fetch URL '%s': HTTP %ld\n", url, rc);
	
	return ret;
}

void fetchurl(const char* url, const char* file) {
	FILE* fp;
	int rc;

	if (!curl) curlsetup();
	
	fp = fopen(file, "wb");
	if (!fp) die("Could not open file '%s' for writing: %s\n", file, strerror(errno));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &fetchurlfile);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	rc = curl_easy_perform(curl);
	if (rc) die("Could not fetch URL '%s' to file '%s'\n", url, file);
	
	fclose(fp);
}

void fetchfiles(vector<FileDownload>& files) {
	FILE* fp;
	int rc;
	
	if (!curl) curlsetup();

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &fetchurlfile);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
	
	for (unsigned int i = 0; i < files.size(); i++) {
		status("Downloading: %s", files[i].name);
		curl_easy_setopt(curl, CURLOPT_URL, files[i].url);
		
		fp = fopen(files[i].filename, "wb");
		if (!fp) die("Could not open file '%s' for writing: %s\n", files[i].filename, strerror(errno));		
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		
		rc = curl_easy_perform(curl);
		if (rc) die("Could not fetch URL '%s' to file '%s'\n", files[i].url, files[i].filename);
		
		fclose(fp);
		printf("\n");
	}
}



// EOF

