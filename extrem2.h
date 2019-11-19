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
#include "approximation.h"

int glob_max(long* arr, int length, int* out, int step);
int glob_min(long* arr, int length, int* out, int step);
