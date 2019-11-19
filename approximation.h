#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "srez.h"
#include "extrem1.h"
#include "extrem2.h"
using namespace cv;

struct exexex {
	int x;
	int y;
};

int selection_points(exexex* input, int amount, exexex zero, int height, int width, int epsilon, exexex* output);