#include <algorithm>
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
    CmdLine cmd("test-meanshift", ' ', "0.1");
    ValueArg<int> _cam_idx("c", "camera", "Specify camera index", false, 0, "INT", cmd);
    ValueArg<int> _width("", "width", "Set frame width", false, 1920, "INT", cmd);
    ValueArg<int> _height("", "height", "Set frame height", false, 1080, "INT", cmd);
    ValueArg<int> _fps("", "fps", "Set frames per second", false, 30, "INT", cmd);
    SwitchArg _v4l2("", "v4l2", "Use v4l2 backend", cmd, false);
    SwitchArg _fullscreen("f", "fullscreen", "Set fullscreen mode", cmd, false);
    ValueArg<std::string> _fmt("", "format", "Set pixel format", false, "", "STRING", cmd);
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

struct MeanShiftTracker {
  enum class Status { Idle, Tracking, Setting };
  Status status;
  int channels[1];
  int hist_size[1];
  float hist_range[2];
  cv::Rect window;
  cv::Mat roi_hist;
  cv::TermCriteria criteria;

  // Default constructor
  MeanShiftTracker();

  // Sets the target in `frame` to track
  void set_target(cv::Mat& frame);
};

MeanShiftTracker::MeanShiftTracker()
    : status(Status::Setting),
      channels{0},
      hist_size{180},
      hist_range{0, 180},
      window(0, 0, 0, 0),
      criteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 10, 1) {}

void MeanShiftTracker::set_target(cv::Mat& frame) {
  cv::Mat roi = frame(window);
  cv::Mat roi_hsv;
  cv::Mat roi_mask;
  cv::cvtColor(roi, roi_hsv, cv::COLOR_BGR2HSV);
  cv::inRange(roi_hsv, cv::Scalar(0, 60, 32), cv::Scalar(180, 255, 255), roi_mask);
  const float* ranges[] = {hist_range};
  cv::calcHist(&roi_hsv, 1, channels, roi_mask, roi_hist, 1, hist_size, ranges);
  cv::normalize(roi_hist, roi_hist, 0, 255, cv::NORM_MINMAX);
}

void on_mouse(int event, int x, int y, int flags, void* param) {
  constexpr int MIN_SIZE = 10;
  MeanShiftTracker* tracker = (MeanShiftTracker*)param;
  switch (event) {
  default:
    break;
  case cv::EVENT_LBUTTONDOWN:
    tracker->status = MeanShiftTracker::Status::Setting;
    tracker->window.x = x;
    tracker->window.y = y;
    tracker->window.width = 0;
    tracker->window.height = 0;
    break;
  case cv::EVENT_LBUTTONUP:
    tracker->status = MeanShiftTracker::Status::Idle;
    // Makes sure the width and height big enough.
    tracker->window.width = std::max(MIN_SIZE, tracker->window.width);
    tracker->window.height = std::max(MIN_SIZE, tracker->window.height);
    break;
  case cv::EVENT_MOUSEMOVE:
    if (flags & cv::EVENT_FLAG_LBUTTON) {
      tracker->window.width = x - tracker->window.x;
      tracker->window.height = y - tracker->window.y;
    }
    break;
  }
}

int main(int argc, char** argv) {
  Args args = handle_args(argc, argv);

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

  std::string win_name = "test-meanshift";
  cv::namedWindow(win_name);
  if (args.fullscreen) {
    cv::setWindowProperty(win_name, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
  }

  MeanShiftTracker tracker;
  cv::setMouseCallback(win_name, on_mouse, &tracker);

  cv::Mat frame;
  while (true) {
    cap.read(frame);

    if (tracker.status == MeanShiftTracker::Status::Idle) {
      // The target window is set, set the target to track.
      tracker.set_target(frame);
      tracker.status = MeanShiftTracker::Status::Tracking;
    }

    if (tracker.status == MeanShiftTracker::Status::Tracking) {
      cv::Mat frame_hsv;
      cv::Mat frame_dst;
      cv::cvtColor(frame, frame_hsv, cv::COLOR_BGR2HSV);
      const float* ranges[] = {tracker.hist_range};
      cv::calcBackProject(&frame_hsv, 1, tracker.channels, tracker.roi_hist, frame_dst, ranges);
      cv::meanShift(frame_dst, tracker.window, tracker.criteria);
      // cv::CamShift(frame_dst, tracker.window, tracker.criteria);
    }

    switch (tracker.status) {
    case MeanShiftTracker::Status::Idle:
      break;
    case MeanShiftTracker::Status::Setting:
      cv::rectangle(frame, tracker.window, cv::Scalar(255, 0, 0), 2);
      break;
    case MeanShiftTracker::Status::Tracking:
      cv::rectangle(frame, tracker.window, cv::Scalar(0, 255, 0), 2);
      break;
    }
    cv::imshow(win_name, frame);

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