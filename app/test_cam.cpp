#include <string>

#include <opencv2/opencv.hpp>
#include <utils/println.h>

int process() {
  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    ERROR("Unable to open default camera");
    return -1;
  }

  std::string win_name = "test-cam";
  cv::namedWindow(win_name);

  cv::Mat frame;
  while (true) {
    cap.read(frame);
    if (!frame.empty()) {
      cv::imshow(win_name, frame);
    }

    int key = cv::pollKey();
    switch (key) {
    default:
      INFO("pressed key: %d", key);
      break;
    case -1:  // No key
      break;
    case 27:  // ESC
      cv::destroyAllWindows();
      cap.release();
      return 0;
    }
  }
}

int main(int argc, char** argv) {
  int exit_code = process();
  return exit_code;
}