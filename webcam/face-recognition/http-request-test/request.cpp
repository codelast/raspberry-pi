#include <string.h>
#include <string>
#include <iostream>
#include "curl/curl.h"
#include "curl/easy.h"

/**
 * A program to test posting http request to Face++ cloud service & output the returned result.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

string apiKey = "your_faceplusplus_api_key";
string apiSecret = "your_faceplusplus_api_secret";
string localImageFilePath = "your_local_image_file_path_to_post_to_faceplusplus_service";
string retData;

size_t writeFunc(char *data, size_t size, size_t nmemb, string *writerData) {
  if (writerData == NULL) {
    return 0;
  }
  size_t len = size * nmemb;
  writerData->append(data, len);
  return len;
}

int main(int argc, char **argv) {
  curl_global_init(CURL_GLOBAL_ALL);
  CURL *curl = curl_easy_init();
  CURLcode res;

  struct curl_httppost *firstItem = NULL;
  struct curl_httppost *lastItem = NULL;

  curl_formadd(&firstItem,
	       &lastItem,
	       CURLFORM_COPYNAME, "api_key", 
	       CURLFORM_COPYCONTENTS, apiKey.c_str(), 
	       CURLFORM_END);
  curl_formadd(&firstItem,
	       &lastItem,
	       CURLFORM_COPYNAME, "api_secret",
	       CURLFORM_COPYCONTENTS, apiSecret.c_str(),
	       CURLFORM_END);

  char* imageFileData = NULL;
  long imageFileSize = 0;
	
  FILE* fp = fopen(localImageFilePath.c_str(), "rb");
  if (fp) {
    fseek(fp, 0, SEEK_END);
    imageFileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    imageFileData = new char[imageFileSize + 1];
    fread(imageFileData, 1, imageFileSize, fp);
    fclose(fp);
  }
	
  curl_formadd(&firstItem, &lastItem,
	       CURLFORM_COPYNAME, "img",
	       CURLFORM_BUFFER, "1.jpg",
	       CURLFORM_BUFFERPTR, imageFileData,
	       CURLFORM_BUFFERLENGTH, imageFileSize,
	       CURLFORM_CONTENTTYPE, "image/jpeg",
	       CURLFORM_END);
	
  if(curl) {
    char errorMsg[512];
    memset(errorMsg, 0, sizeof(errorMsg));
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorMsg);

    curl_easy_setopt(curl, CURLOPT_URL, "http://apicn.faceplusplus.com/v2/detection/detect");  // Face++ service URL
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, firstItem);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &retData);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunc);

    res = curl_easy_perform(curl);
    if(CURLE_OK != res) {
      cout << "Error occurred: " << errorMsg << ", error code: " << res << endl;
    }
  }

  curl_formfree(firstItem);
  curl_easy_cleanup(curl);
  curl = NULL;
  cout << retData << endl;

  curl_global_cleanup();
  if(imageFileData != NULL) {
    delete [] imageFileData;
    imageFileData = NULL;
  }

  return 0;
}
