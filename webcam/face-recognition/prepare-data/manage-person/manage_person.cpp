#include <string.h>
#include <string>
#include <iostream>
#include "curl/curl.h"
#include "curl/easy.h"

/**
 * A program to add/delete Face++ Person.
 *
 * @author Darran Zhang @ codelast.com
 */

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
  if (argc < 3) {
    cout << "Usage: ./manage_person [your_faceplusplus_api_key] [your_faceplusplus_api_secret] [create/delete] [person_name]" << endl;
    return 1;
  }
  string apiKey = argv[1];
  string apiSecret = argv[2];
  string operationType = argv[3];  // should be "create" or "delete"
  string personName = argv[4];

  string retData;  // the data returned by Face++ service

  if (operationType != "create" && operationType != "delete") {
    cout << "The 1st param should be [create] or [delete]";
    return 1;
  }

  string faceplsuplusURL= "https://apicn.faceplusplus.com/v2/person/" + operationType;

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
  curl_formadd(&firstItem,
	       &lastItem,
	       CURLFORM_COPYNAME, "person_name",
	       CURLFORM_COPYCONTENTS, personName.c_str(),
	       CURLFORM_END);

  if(curl) {
    char errorMsg[512];
    memset(errorMsg, 0, sizeof(errorMsg));
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorMsg);

    curl_easy_setopt(curl, CURLOPT_URL, faceplsuplusURL.c_str());  // Face++ service URL
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

  return 0;
}
