#ifndef __WEBCAM_CONTROL_H
#define __WEBCAM_CONTROL_H

#include <string>
#include "opencv.hpp"

/**
 * A class to control the webcam to capture images.
 *
 * @author Darran Zhang @ codelast.com
 */

class WebcamControl
{
 public:
  WebcamControl();
  virtual ~WebcamControl();

 private:
  CvCapture* pCapture;
  
  std::string getImageFilePath(std::string imageDir);

 public:
  bool init(int webcamIndex);
  bool captureImage(std::string imageDir);
  void cleanup();
};

#endif
