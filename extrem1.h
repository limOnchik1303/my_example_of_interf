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
#include "extrem2.h"
#include "approximation.h"

int func_min(long* arr, int length, int* ext, int step);
int func_max(long* arr, int length, int* ext, int step);
long standard_deviation(long* arr, int length, int x0, int step);
void averaging(long* input, int length, int step, long* output);