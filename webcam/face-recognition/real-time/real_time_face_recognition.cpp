#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include "curl/curl.h"
#include "curl/easy.h"
#include "opencv.hpp"

/**
 * A program to verify the face in front of an USB camera when motion is detected.
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

string getImageFileFaceId(string apiKey,
			  string apiSecret,
			  string localImageFilePath,
			  string remoteImageFileName) {
  string retData;  // the data returned by Face++ service

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

    curl_easy_setopt(curl, CURLOPT_URL, "https://apicn.faceplusplus.com/v2/detection/detect");  // Face++ service URL
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

  if(imageFileData != NULL) {
    delete [] imageFileData;
    imageFileData = NULL;
  }

  /* parse the JSON data returned by Face++ serivce */
  Document document;
  if (document.Parse(retData.c_str()).HasParseError()) {
    cout << "Failed to parse the Face++ returned data: [" << retData << "]" << endl;
    return "";
  }
  const Value& faces = document["face"];
  if (0 == faces.Size()) {  // maybe no face was detected in the image
    cout << "No face was detected in the image" << endl;
    return "";
  }

  return faces[0]["face_id"].GetString();
}

bool verifyFaceId(string apiKey,
		  string apiSecret,
		  string personName,
		  string faceId2Verify) {
  string retData;  // the data returned by Face++ service
  string faceplsuplusURL= "https://apicn.faceplusplus.com/v2/recognition/verify";

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
  curl_formadd(&firstItem,
	       &lastItem,
	       CURLFORM_COPYNAME, "face_id",
	       CURLFORM_COPYCONTENTS, faceId2Verify.c_str(),
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
  cout << retData << endl;

  /* resource cleanup */
  curl_formfree(firstItem);
  curl_easy_cleanup(curl);
  curl = NULL;

  /* parse the JSON data returned by Face++ serivce */
  Document document;
  if (document.Parse(retData.c_str()).HasParseError()) {
    cout << "Failed to parse the Face++ returned data: [" << retData << "]" << endl;
    return false;
  }
  return document["is_same_person"].GetBool();
}

int main(int argc, char **argv) {
  if (argc < 7) {
    cout << "Usage : ./real_time_face_recognition [motion_detect_threshold] [your_faceplusplus_api_key] [your_faceplusplus_api_secret] [person_name] [usb_wbcam_captured_local_image_file_path] [remote_image_file_name]";
    return 1;
  }

  int detectThreshold = atoi(argv[1]);
  string apiKey = argv[2];
  string apiSecret = argv[3];
  string personName = argv[4];
  string localImageFilePath = argv[5];
  string remoteImageFileName = argv[6];

  string retData;  // the data returned by Face++ service

  /* init camera */
  CvCapture* pCapture = cvCreateCameraCapture(0);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH, 640);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT, 480);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_BRIGHTNESS, 20);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_CONTRAST, 10);

  if (NULL == pCapture) {
    cout << "Can't initialize webcam!" << endl;
    return 1;
  }

  curl_global_init(CURL_GLOBAL_ALL);

  IplImage *pFrame = cvQueryFrame(pCapture);
  IplImage *pFrameA = NULL;
  IplImage *pFrameB = cvCreateImage(cvSize(pFrame->width, pFrame->height), pFrame->depth, pFrame->nChannels);
  IplImage *pFrameSub = cvCloneImage(pFrameB);

  int nDims = 256;
  float hRangesArr[] = {0, 255};
  float* hRanges = hRangesArr;

  IplImage *pGrayscaleImage = NULL;
  CvHistogram *pHist = cvCreateHist(1, &nDims, CV_HIST_ARRAY, &hRanges, 1);
  float fMaxValue = 0.0;

  time_t ts = 0;  // used to record current timestamp & prevent triggering the monition detection for multi times in a time peroid
  while(true) {
    pFrameA = cvQueryFrame(pCapture);
    if(!pFrameA) {
      fprintf(stdout, "Can't grab images!\n");
 
      break;
    }

    cvAbsDiff(pFrameB, pFrameA, pFrameSub);  // calculate the diff of two images
    cvCopy(pFrameA, pFrameB);                // copy image, the 1st param is source & the 2nd is dest

    pGrayscaleImage = cvCreateImage(cvGetSize(pFrameSub), IPL_DEPTH_8U, 1);
    cvCvtColor(pFrameSub, pGrayscaleImage, CV_BGR2GRAY);
    cvCalcHist(&pGrayscaleImage, pHist, 0, 0);

    fMaxValue = 0.0;
    cvGetMinMaxHistValue(pHist, 0, &fMaxValue, 0, 0);
    cvConvertScale(pHist->bins, pHist->bins, (fMaxValue ? (255.0 / fMaxValue) : 0.0), 0);

    double dRealtimeVal = cvGetReal1D(pHist->bins, 10);
    if (dRealtimeVal > detectThreshold) {  // triggered
      time_t currentTimestamp = time(NULL);
      if (currentTimestamp - ts >= 3) {
	ts = currentTimestamp;
	cout << endl << "Motion detected @ " << ctime(&currentTimestamp);

	sleep(1);  // sleep for a while to wait for a stable position of the person in front of the USB webcam
	pFrame = cvQueryFrame(pCapture);  // capture an image
	cvSaveImage(localImageFilePath.c_str(), pFrame);  // save the image file
	// invoke Face++ service to get the face_id of the image file
	string faceId = getImageFileFaceId(apiKey, apiSecret, localImageFilePath, remoteImageFileName);
	if (faceId.empty()) {
	  continue;
	}
	// invoke Face++ service to verify the face_id
	cout << "Face detected! will verify face_id [" << faceId << "] ..." << endl;
	if (verifyFaceId(apiKey, apiSecret, personName, faceId)) {
	  cout << "+++ Verification OK!" << endl;
	} else {
	  cout << "--- WARNING: Verification failed!" << endl;
	}
      }
    }

    cvReleaseImage(&pGrayscaleImage);  // free memory
    pGrayscaleImage = NULL;
 
    cvWaitKey(10);  // wait for N milliseconds
  }

  cvReleaseCapture(&pCapture);  // stop capturing images & release resources
  cvReleaseHist(&pHist);
  cvReleaseImage(&pFrameB);
  cvReleaseImage(&pFrameSub);

  pCapture = NULL;
  pHist = NULL;
  pFrameB = NULL;
  pFrameSub = NULL;
  pFrame = NULL;
  pFrameA = NULL;

  curl_global_cleanup();

  return 0;
}
