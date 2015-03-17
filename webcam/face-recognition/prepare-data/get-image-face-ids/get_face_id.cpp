#include <string.h>
#include <string>
#include <iostream>
#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include "curl/curl.h"
#include "curl/easy.h"

/**
 * A program to get the face_id for an image file.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace rapidjson;
using namespace std;

size_t writeFunc(char *data, size_t size, size_t nmemb, string *writerData) {
  if (writerData == NULL) {
    return 0;
  }
  size_t len = size * nmemb;
  writerData->append(data, len);
  return len;
}

int main(int argc, char **argv) {
  if (argc < 5) {
    cout << "Usage : ./get_face_id [your_faceplusplus_api_key] [your_faceplusplus_api_secret] [local_image_file_path_to_post_to_get_face_id] [remote_image_file_name]";
    return 1;
  }

  string apiKey = argv[1];
  string apiSecret = argv[2];
  string localImageFilePath = argv[3];
  string remoteImageFileName = argv[4];

  string retData;  // the data returned by Face++ service
  
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
	       CURLFORM_BUFFER, remoteImageFileName.c_str(),
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

  /* resource cleanup */
  curl_formfree(firstItem);
  curl_easy_cleanup(curl);
  curl = NULL;

  curl_global_cleanup();
  if(imageFileData != NULL) {
    delete [] imageFileData;
    imageFileData = NULL;
  }

  /* parse the JSON data returned by Face++ serivce & print the face_id */
  Document document;
  if (document.Parse(retData.c_str()).HasParseError()) {
    return 1;
  }
  const Value& faces = document["face"];
  cout << faces[0]["face_id"].GetString() << endl;

  return 0;
}
