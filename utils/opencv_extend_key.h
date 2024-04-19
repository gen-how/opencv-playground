#ifndef _OPENCV_EXTEND_KEY_H
#define _OPENCV_EXTEND_KEY_H

namespace cv {

/* These key codes are os-dependent. */
#if defined(__APPLE__)
constexpr int KEY_UP = 63232;
constexpr int KEY_DOWN = 63233;
constexpr int KEY_LEFT = 63234;
constexpr int KEY_RIGHT = 63235;
#elif defined(__linux__)
constexpr int KEY_UP = 65362;
constexpr int KEY_DOWN = 65364;
constexpr int KEY_LEFT = 65361;
constexpr int KEY_RIGHT = 65363;
#elif defined(__MSVC__)
constexpr int KEY_UP = 2490368;
constexpr int KEY_DOWN = 2621440;
constexpr int KEY_LEFT = 2424832;
constexpr int KEY_RIGHT = 2555904;
#endif

}  // namespace cv

#endif  // _OPENCV_EXTEND_KEY_H
