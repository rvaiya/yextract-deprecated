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
#include "aux.h"
#include "vector.h"
#define DEBUG

struct link {
	char *url;
	char *quality;
};

void link_free(void *link) {
	free(((struct link*)link)->quality);
	free(((struct link*)link)->url);
	free(link);
}

struct link* processlink(char *str) {
	struct link *result=malloc(sizeof(struct link));
	char *qualityraw, *urlraw, *sigraw;

	sigraw=getparam(str, "sig");
	if(!(sigraw))  return NULL;

	urlraw=getparam(str, "url");
	if(!(urlraw))  return NULL;

	qualityraw=getparam(str, "quality");
	if(!(qualityraw)) return NULL;
	
	//Append signature to media url	
	int s_url=strlen(urlraw)+strlen(sigraw)+strlen("&signature=")+1;
	char *finalurl=malloc(s_url*sizeof(char));
	strcpy(finalurl, urlraw);
	strcat(finalurl, "&signature=");
	strcat(finalurl, sigraw);

	result->url=pdecode(finalurl);
	result->quality=pdecode(qualityraw);
	free(urlraw);
	free(finalurl);
	free(qualityraw);
	free(sigraw);
	return result;
}

//Returns vector of link pointers for a youtube video with the given id
Vector getlinks(char *id) 
{
	char *format="www.youtube.com/get_video_info?&video_id=%s&el=detailpage&ps=default&eurl=&gl=US&hl=en";
	char *url=malloc(sizeof(char)*(strlen(id)+strlen(format)-1));
	sprintf(url, format, id);
	char *content=gethtml(url);
	free(url);
	if (content == NULL) {
		#ifdef DEBUG
			fprintf(stderr, "Failed to retrieve content at %s\n", format);
		#endif 
		return NULL;
	}

	#ifdef DEBUG
		FILE *fp=fopen("content.log", "w");
		if(fp)
			fprintf(fp, "%s", content);
		else {
			fprintf(stderr, "failed to create content.log\n");
			return NULL;
			fclose(fp);
		}
		fclose(fp);
	#endif

	//Extract raw link data
	char *rawlinks=getparam(content,"url_encoded_fmt_stream_map");
	if(!rawlinks) {
		#ifdef DEBUG
			fprintf(stderr, "Cannot find param url_encoded_fmt_stream_map\n");
		#endif
		return NULL;
	}
	char *temp=rawlinks;
	rawlinks=pdecode(rawlinks);
	free(temp);

	//Process links 
	char *link;
	Vector result=vector_init(link_free);
	link=strtok(rawlinks, ",");
	if(!link) return NULL;
	do {
		struct link *plink=processlink(link);
		if(!plink) return NULL;
		vector_push(result, plink);
	} while((link=strtok(NULL, ",")));
	free(rawlinks);
	return result;
}


int main(int argc, char **argv)
{
	char *url;
	if (argc < 2 ) {
		printf("Usage: %s <youtube link>\n", argv[0]);
		return 1;
	} else if(argc == 3) 
		url=argv[2];
	 else
		url=argv[1];

	char *id=getparam(url, "v");
	if (id == NULL) {
		printf("Invalid youtube link\n");
		return 2;
	}
	Vector links=getlinks(id);
	free(id);
	if (links == NULL) {
		printf("Unable to parse link (please ensure you've entered a valid youtube link)\n");
		return 2;
	}
	if(argc == 3 && !strcmp(argv[1], "-a")) {
		struct link *i;
		while(i=vector_pop(links)) { 
			printf("%s\t%s\n", i->quality,i->url);
			link_free(i);
		}
	}
	else  
		printf("%s\n", ((struct link*)links->data[0])->url);
	vector_delete(links);
	return 0;
}
