#include <stdio.h>

void processus2() {
  for (;;) {
    sleep(1);
    printf("Hello, world from P2\n");
  }
}

void processusX(int x) {
  for (int i = 0; i < x; i++) {
    printf("Hello, world from P%d\n", x);
  }
}

void processus3() {
  for (;;) {
    sleep(2);
    printf("Hello, world from P3\n");
  }
}

void processus4() {
  for (;;) {
    sleep(5);
    printf("Hello, world from P4\n");
  }
}

void processus5() {
  for (;;) {
    sleep(5);
    printf("Hello, world from P5\n");
  }
}

void processus6() {
  for (;;) {
    printf("Hello, world from P6\n");
  }
}

void processus7() {
  for (;;) {
    printf("Hello, world from P7\n");
  }
}

void processus8() {
  for (;;) {
    printf("Hello, world from P8\n");
  }
}

void processus9() {
  for (;;) {
    printf("Hello, world from P9\n");
  }
}