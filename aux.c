#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *regexend(char *string, const char *regex) 
{
	regex_t mreg;
	regcomp(&mreg,regex, REG_EXTENDED);
	regmatch_t *m=malloc(sizeof(regmatch_t));
	if (regexec(&mreg, string, 1,m,0)) {
		regfree(&mreg);
		free(m);
		return NULL;
	}
	char *match=string+m->rm_eo;
	regfree(&mreg);
	free(m);
	return match;
}


int strnoc(char *text, const char *sub) 
{
	int i=0;
	char *temp=text;
	while((temp=strstr(temp, sub)) != NULL)  { i++; temp++; }
	return i;
}

int substr(char **text, const char *sub, const char *repl) 
{
	int l_repl, l_sub;
	l_repl=strlen(repl);
	l_sub=strlen(sub);
	char *match;
	if (l_repl > l_sub) {
		int nsize=(l_repl-l_sub)*strnoc(*text, sub)+strlen(*text) + 1;
		*text=realloc(*text, sizeof(char)*nsize);
		match=*text;
		while((match=strstr(match, sub)) != NULL) {
			memmove(match+l_repl, match+l_sub, strlen(match+l_sub)+1);
			memcpy(match, repl, l_repl);
			match+=1; 
		}

	} else {
		match=*text;
		while((match=strstr(match, sub)) != NULL) {
			memcpy(match, repl, strlen(repl));
			memmove(match+l_repl, match+l_sub, strlen(match+l_sub) + 1);
		}
	}
}


void normalize(char **url) //Inplace modification
{
	substr(url, "\%3A", ":");
	substr(url, "\%26", "&");
	substr(url, "\%2F", "/");
	substr(url, "\%3D", "=");
	substr(url, "\%3F", "?");
	substr(url, "\%3B", ";");
	substr(url, "\%2C", ",");
	substr(url, "\%2B", ",");
	substr(url, "\%252B", "+");
	substr(url, "\%252C", ",");
	substr(url, "\%253B", ";");
	substr(url, "\%253A", ":");
	substr(url, "\%2526", "&");
	substr(url, "\%252F", "/");
	substr(url, "\%253D", "=");
	substr(url, "\%253F", "?");
	substr(url, "\%25252C", ",");
	substr(url, "sig", "signature");
}

