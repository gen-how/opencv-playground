# OpenCV Playground

## Build

```shell
cmake -B build -S .
```
如果 OpenCV 不是裝載預設路徑, 可以手動提供:

```shell
cmake -B build -S . -DCMAKE_PREFIX_PATH="/opt/opencv-4.9.0-release;"
```

## Usage

```shell
./build/test-cam
```