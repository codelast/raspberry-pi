#include <sstream>
#include <glog/logging.h>
#include "util.h"
#include "webcam_control.h"

/**
 * A class to control the webcam to capture images.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

WebcamControl::WebcamControl() {
  pCapture = NULL;
}

WebcamControl::~WebcamControl() {
}

/** 
 * Initialize the webcam.
 * 
 * @param webcamIndex The index of the webcam in OS, if there is only one webcam connected with Raspberry Pi, then the index is 0.
 * @return true for successfully initialize the webcam, false otherwise.
 */
bool WebcamControl::init(int webcamIndex) {
  pCapture = cvCreateCameraCapture(0);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH, 640);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT, 480);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_BRIGHTNESS, 20);
  cvSetCaptureProperty(pCapture, CV_CAP_PROP_CONTRAST, 10);

  if (NULL == pCapture) {
    LOG(ERROR) << "Can't initialize webcam!";
    return false;
  }

  return true;
}

/** 
 * Use the webcam to capture an image & save it to the specified directory.
 *
 * @param imageDir       A local dir to save the images captured by the webcam.
 * @param imageFilePath  The returned path of the image file which we saved.
 */
bool WebcamControl::captureImage(const string &imageDir, string &imageFilePath) {
  if (NULL == pCapture) {
    LOG(ERROR) << "Can't capture image because webcam was not properly initialized!";
    return false;
  }

  IplImage *pFrame = cvQueryFrame(pCapture);  // query a frame
  if(NULL == pFrame) {
    LOG(ERROR) << "Can't get a frame from webcam!";
    return false;
  }

  cvSaveImage(getImageFilePath(imageDir).c_str(), pFrame);

  return true;
}

/** 
 * Get the path of a image file to write.
 *
 * @param imageDir  A local dir to save the images captred by the webcam.
 * @return The image file path.
 */
string WebcamControl::getImageFilePath(const string &imageDir) {
  stringstream ss;
  ss << imageDir << "/" << CUtil::getMillisecond() << ".jpg";
  return ss.str();
}

/** 
 * Free OpenCV resources.
 *
 */
void WebcamControl::cleanup() {
  if (NULL == pCapture) {
    LOG(INFO) << "Nothing to free";
    return;
  }
  cvReleaseCapture(&pCapture);  // free memory
}
