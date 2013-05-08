#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

size_t csize;
char *content;
size_t handledata( char *ptr, size_t size, size_t nmemb, void *userdata) 
{
	size_t osize=csize;
	csize+=size*nmemb;
	content=realloc(content, csize);
	memcpy(content+osize-1, ptr, nmemb*size);
	content[csize-1]='\0';
	return nmemb*size;
}

char *gethtml(char *url)
{
	content=NULL;
	csize=1;
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *sh=curl_easy_init();
	curl_easy_setopt(sh, CURLOPT_URL, url);
	curl_easy_setopt(sh, CURLOPT_WRITEFUNCTION, handledata);
	curl_easy_setopt(sh, CURLOPT_WRITEDATA, NULL);
	if(curl_easy_perform(sh)) {
		free(content);
		curl_easy_cleanup(sh);
		curl_global_cleanup();
		return NULL;
	}
	curl_easy_cleanup(sh);
	curl_global_cleanup();
	return content;
}
