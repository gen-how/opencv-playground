add_executable(test-cam
    test_cam.cpp
)
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


add_executable(test-meanshift
    test_meanshift.cpp
)
set_target_properties(test-meanshift PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_include_directories(test-meanshift
    PUBLIC "${CMAKE_SOURCE_DIR}"
    PUBLIC "${OpenCV_INCLUDE_DIRS}"
)
target_link_libraries(test-meanshift
    PUBLIC ${OpenCV_LIBS}
)