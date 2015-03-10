#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "opencv.hpp"

/**
 * A program to detect motion in front of an USB camera using OpenCV.
 *
 * @author Darran Zhang @ codelast.com
 */

int main(int argc, char **argv) {
  int detectThreshold = 19;
  if (argc >= 2) {
    detectThreshold = atoi(argv[1]);
    printf("=== Motion detection threshold has been set to: [%d] ===\n", detectThreshold);
  }

  /* init camera */
  CvCapture* pCapture = cvCreateCameraCapture(0);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH, 640);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT, 480);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_BRIGHTNESS, 20);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_CONTRAST, 10);

  if (NULL == pCapture) {
    fprintf(stderr, "Can't initialize webcam!\n");
    return 1;
  }

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

  time_t ts = 0;  // used to record current timestamp & prevent triggering the monition detection for multi times in 1 second
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
      if (currentTimestamp - ts >= 1) {
	ts = currentTimestamp;
	printf("Motion detected @ %s", ctime(&currentTimestamp));
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

  return 0;
}
