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

char *getlink(char *id)
{
	char *format="www.youtube.com/get_video_info?&video_id=%s&el=detailpage&ps=default&eurl=&gl=US&hl=en";
	char *url=malloc(sizeof(char)*(strlen(id)+strlen(format)-1));
	sprintf(url, format, id);
	char *content=gethtml(url);
	free(url);
	if (content == NULL) return NULL;
	#ifdef DEBUG
		fprintf(stderr, "%s", content);
	#endif
	char *begin="url_encoded_fmt_stream_map=itag[^h]*";
	char *end="itag\%3D";
	char *start=regexend(content,begin);
	if (start == NULL || start+1 == '\0') return NULL;
	char *stop=strstr(start+1, end);
	if (stop == NULL) return NULL;
	const int len=stop-start;
	char *result=malloc((len+2)*sizeof(char));
	memcpy(result, start, len);
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
