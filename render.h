#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <algorithm>
#include <stdlib.h>
#include "render.cpp"

struct point;
struct triangle;
struct obj;

void prntScreen();

void renderObj();

void rotateObj();

void shiftObj();

#endif
