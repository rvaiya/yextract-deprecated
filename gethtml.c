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
#include <curl/curl.h>
#include <stdlib.h> 
#include <string.h> 

static char *content=NULL;
static int htmllen=0;

int gethtml_dump(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	content=realloc(content, (nmemb+htmllen)*size);
	char *temp=content+htmllen;
	htmllen+=nmemb;
	memcpy(temp, ptr, nmemb);
	return nmemb*size;
}

char *gethtml(char *url) //Returns NULL if url is invalid/cannot be retrieved
{
	content=NULL;
	htmllen=0;
	CURL *sh=curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);
	curl_easy_setopt(sh, CURLOPT_URL, url);
	curl_easy_setopt(sh, CURLOPT_WRITEFUNCTION, &gethtml_dump);
	if (curl_easy_perform(sh))  {
		curl_easy_cleanup(sh);
		return NULL;
	}
	curl_easy_cleanup(sh);
	content=realloc(content, htmllen+1);
	content[htmllen]='\0';
	return content;
}

