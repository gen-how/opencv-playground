#include <string>

#include <opencv2/opencv.hpp>
#include <tclap/CmdLine.h>
#include <utils/println.h>

struct Args {
  int cam_idx;
  int width;
  int height;
  int fps;
  bool v4l2;
  bool fullscreen;
  std::string fmt;
};

Args handle_args(int argc, char** argv) {
  using namespace TCLAP;
  Args args;
  try {
    CmdLine cmd("test-cam", ' ', "0.1");
    ValueArg<int> _cam_idx("c", "camera", "camera index", false, 0, "int", cmd);
    ValueArg<int> _width("", "width", "frame width", false, 1920, "int", cmd);
    ValueArg<int> _height("", "height", "frame height", false, 1080, "int", cmd);
    ValueArg<int> _fps("", "fps", "frame per second", false, 30, "int", cmd);
    SwitchArg _v4l2("", "v4l2", "use v4l2 backend", cmd, false);
    SwitchArg _fullscreen("f", "fullscreen", "fullscreen mode", cmd, false);
    ValueArg<std::string> _fmt("", "format", "pixel format", false, "", "string", cmd);
    cmd.parse(argc, argv);
    args.cam_idx = _cam_idx.getValue();
    args.width = _width.getValue();
    args.height = _height.getValue();
    args.fps = _fps.getValue();
    args.v4l2 = _v4l2.getValue();
    args.fullscreen = _fullscreen.getValue();
    args.fmt = _fmt.getValue();
  } catch (ArgException& e) {
    ERROR("%s for arg %s", e.error().c_str(), e.argId().c_str());
    exit(1);
  }
  return args;
}

int process(Args args) {
  cv::VideoCapture cap(args.cam_idx, args.v4l2 ? cv::CAP_V4L2 : cv::CAP_ANY);
  if (!cap.isOpened()) {
    ERROR("Unable to open camera(%d)", args.cam_idx);
    return -1;
  }
  if (!args.fmt.empty()) {
    INFO("Set pixel format: %s", args.fmt.c_str());
    cap.set(cv::CAP_PROP_FOURCC,
            cv::VideoWriter::fourcc(args.fmt[0], args.fmt[1], args.fmt[2], args.fmt[3]));
  }
  cap.set(cv::CAP_PROP_FRAME_WIDTH, args.width);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, args.height);
  cap.set(cv::CAP_PROP_FPS, args.fps);

  std::string win_name = "test-cam";
  cv::namedWindow(win_name);
  if (args.fullscreen) {
    cv::setWindowProperty(win_name, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
  }

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
  Args args = handle_args(argc, argv);

  return process(args);
}