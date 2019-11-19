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
#include <math.h>
#include "srez.h"
#include "extrem1.h"
#include "extrem2.h"
#include "approximation.h"
#include "approximation_function.h"
using namespace cv;

struct {
	int x;
	int y;
	double dif;
	double koef;
} typedef best_cut;

int axis_gravity(int** input, int width, int height, int delta, exexex* output);
best_cut search_best_corner(int** input, exexex point, double edge, double step, int* flag);
void search_best_section_x(int** input, exexex* centre, int amount, double edge, double step, best_cut* output, int amount_points);