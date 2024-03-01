#ifndef PRINTLN_H
#define PRINTLN_H

#include <stdio.h>

#define println(fmt, ...)            \
  do {                               \
    printf(fmt "\n", ##__VA_ARGS__); \
  } while (0)

#define eprintln(fmt, ...)                    \
  do {                                        \
    fprintf(stderr, fmt "\n", ##__VA_ARGS__); \
  } while (0)

#define ERROR(fmt, ...) eprintln("error: " fmt, ##__VA_ARGS__)
#define INFO(fmt, ...)  println("info: " fmt, ##__VA_ARGS__)
#define WARN(fmt, ...)  println("warn: " fmt, ##__VA_ARGS__)

#endif  // PRINTLN_H