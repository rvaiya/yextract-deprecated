/********************************************************************
Author: Raheman Vaiya
Email: r.vaiya@gmail.com

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gethtml.h"


char *getid(char *url)
{
	const size_t maxidlen=15;
	char *result=malloc((maxidlen+1)*sizeof(char));
	char *id=strstr(url, "v=");
	if (id == NULL) return NULL;
	int i;
	id+=2;
	for(i=0;i<maxidlen & id[i] != '\0' && id[i] != '&';i++) { result[i]=id[i]; }
	result[i]='\0';
	return result;
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

char *getlink(char *id)
{
	char *format="www.youtube.com/get_video_info?&video_id=%s&el=detailpage&ps=default&eurl=&gl=US&hl=en";
	char *url=malloc(sizeof(char)*(strlen(id)+strlen(format)-1));
	sprintf(url, format, id);
	char *content=gethtml(url);
	free(url);
	if (content == NULL) return NULL;
	char *begin="url_encoded_fmt_stream_map=itag\%3D46\%26url\%3Dh";
	char *end="itag\%3D";
	char *start=strstr(content, begin);
	if (start == NULL || start+1 == '\0') return NULL;
	start+=strlen(begin);
	char *stop=strstr(start+1, end);
	if (stop == NULL) return NULL;
	const int len=stop-start;
	char *result=malloc((len+2)*sizeof(char));
	memcpy(result+1, start, len);
	result[0]='h';
	result[len]='\0';
	normalize(&result);
	return result;
}


int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s <youtube link>\n", argv[0]);
		return 1;
	}

	char *url=argv[1];
	char *id=getid(url);
	if (id == NULL) {
		printf("Invalid youtube link\n");
		return 2;
	}
	char *purl=getlink(id);
	free(id);
	if (purl == NULL) {
		printf("Unable to parse link (please ensure you've entered a valid youtube link)\n");
		return 2;
	}
	printf("%s\n", purl);
	free(purl);
	return 0;
}
