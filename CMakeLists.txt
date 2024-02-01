cmake_minimum_required(VERSION 3.5)

project("opencv-playground" VERSION 0.0.1 LANGUAGES CXX)

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Please create a separate directory and run 'cmake' from there.")
endif()

# If not provided, set the build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

# Add targets
add_executable(test-cam test_cam.cpp)
set_target_properties(test-cam PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_include_directories(test-cam
    PUBLIC "${CMAKE_SOURCE_DIR}"
    PUBLIC "${OpenCV_INCLUDE_DIRS}"
)
target_link_libraries(test-cam
    PUBLIC ${OpenCV_LIBS}
)