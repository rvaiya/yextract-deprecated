#ifndef _AUX_H_
#define _AUX_H_

char *getbase(char *url);
int strnoc(char *text, char *sub);
int substr(char **text, const char *sub, const char *repl);
char *pdecode(char *url);
char *getparam(char *url, char *param);
#endif
