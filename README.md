# OpenCV Playground

## Build

```shell
cmake -B build -S .
cmake --build "build"
```
如果 OpenCV 不是裝在預設路徑, 可以手動提供:

```shell
cmake -B build -S . -DCMAKE_PREFIX_PATH="/opt/opencv-4.9.0-release;"
cmake --build "build"
```

## Usage

```shell
./build/app/test-cam --help
```

```shell
./build/app/test-meanshift --help
```
