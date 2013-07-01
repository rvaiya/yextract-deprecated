#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Returns file/script url refers to, without params or the site itself if there
//is no trailing slash

char *getbase(char *url)
{
	if(!url) return NULL;
	char *startbase, *endbase;
	endbase=startbase=NULL;
	while(*url != '\0') {
		url++;
		if(*url == '/') {
			startbase=url+1;
			endbase=NULL;
		}
		if(!endbase && (*url == '?' || *url == '\0')) {
			endbase=url;
		}
	}
	if(!startbase) return NULL;
	return strndup(startbase, endbase-startbase);
}


//Returns the number of occurrences of sub in text
int strnoc(char *text, const char *sub) 
{
	int i=0;
	char *temp=text;
	while((temp=strstr(temp, sub)) != NULL)  { i++; temp++; }
	return i;
}


//Substitutes all occurrences of "sub" with "repl" in "text"
void substr(char **text, const char *sub, const char *repl) 
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
			memcpy(match, repl, l_repl);
			memmove(match+l_repl, match+l_sub, strlen(match+l_sub) + 1);
		}
	}
}



//Used by pdecode
char forbidden[] = "!#$&'()*+,/:;=?@[]% ";

int isforbidden(char c) {
	char *s;
	for(s=forbidden;*s != '\0';s++) 
		if(*s == c) return 1;
	return 0;
}

//Decodes percent encoded string
char *pdecode(char *str) {
	char *s;
	int slen=strlen(str);
	char *result=malloc((slen+1)* sizeof(char));
	char *r=result;
	for(s=str;*s!='\0';s++,r++) {
		if(*s == '%' && s+1 != '\0' && s+2 != '\0') {
			int c;
			sscanf(s+1, "%2x", &c);	
			s+=2;
			*r=(char)c;
		}
		else 
			*r=*s;
	}
	*r='\0';
	return result;
}


//Extract parameter from a url
char *getparam(char *str, char *param) {
	//Append = sign
	char *search=malloc((strlen(param)+2)*sizeof(char));
	memcpy(search, param, strlen(param));
	search[strlen(param)]='=';
	search[strlen(param)+1]='\0';

	char *begin=strstr(str, search);
	if(!begin) {
		free(search);
		#ifdef DEBUG
			fprintf(stderr, "Could not parse param %s, from %s\n", param, str);
		#endif
		return NULL;
	}
	begin+=strlen(search);
	char *end=strstr(begin, "&");
	char *result;

	if(end)  //If & found before end of the string, only copy relevant text
		result=strndup(begin, end-begin);
	else 
		result=strdup(begin);

	free(search);
	return result;
}
