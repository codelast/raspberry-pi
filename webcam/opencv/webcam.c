#include <stdio.h>
#include <stdlib.h>
#include "opencv.hpp"

/**
 * A program to capture an image from USB camera using OpenCV.
 *
 * @author Darran Zhang @ codelast.com
 */

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: ./webcam output_image_file_name\n");
    return 1;
  }

  /* init camera */
  CvCapture* pCapture = cvCreateCameraCapture(0);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH, 640);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT, 480);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_BRIGHTNESS, 20);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_CONTRAST, 10);

  IplImage *pFrame = 0;

  if (NULL == pCapture) {
    fprintf(stderr, "Can't initialize webcam!\n");
    return 1;
  }

  pFrame = cvQueryFrame(pCapture);  // query a frame

  if(NULL == pFrame) {
    fprintf(stderr, "Can't get a frame!\n" );
    return 1;
  }

  const char *pImageFileName = argv[1];
  cvSaveImage(pImageFileName, pFrame);

  cvReleaseCapture(&pCapture);  // free memory

  return 0;
}
