#include <iostream>
#include "opencv.hpp"

/**
 * A program to capture video from USB camera & save to a file using OpenCV.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Usage: ./video_capture output_video_file_name\n" << endl;
    return 1;
  }

  string outputVideoFile = argv[1];

  VideoCapture videoCapture(0);  // open the USB webcam
  if (!videoCapture.isOpened()) {
    cout << "Failed to open the default camera" << endl;
    return 1;
  }

  /* get frame size(width, height) */
  double frameWidth = videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
  double frameHeight = videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
  cout << "Frame size is [" << frameWidth << "x" << frameHeight << "]" << endl;

  Size frameSize(static_cast<int>(frameWidth), static_cast<int>(frameHeight));
  VideoWriter videoWriter(outputVideoFile.c_str(), CV_FOURCC('D', 'I', 'V', 'X'), 10, frameSize, true);
  if (!videoWriter.isOpened()) {
    cout << "Failed to initialize the VideoWriter" << endl;
    return 1;
  }

  while (true) {
    Mat frame;
    if (!videoCapture.read(frame)) {  // grabs, decodes and returns the next video frame
      cout << "Failed to read a video frame" << endl;
      break;
    }

    videoWriter.write(frame);  // write the frame to the file
    if (27 == waitKey(30)) {   // wait for the press of "ESC" key(NOTE: not working if no window was created!)
      cout << "ESC key pressed, stop capturing video" << endl;
      break;
    }
  }

  return 0;
}
