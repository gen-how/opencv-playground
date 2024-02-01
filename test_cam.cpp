#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  int dev_id = 0;
  if (argc > 1) {
    dev_id = std::stoi(argv[1]);
  }

  cv::VideoCapture cap(dev_id);
  if (!cap.isOpened()) {
    std::cout << "error: Unable to open camera(" << dev_id << ")\n";
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

    switch (cv::pollKey()) {
    default:
      break;
    case 27:  // ESC
      cv::destroyAllWindows();
      cap.release();
      return 0;
    }
  }
}