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
#include "approximation.h"
using namespace cv;

void additon(exexex* bad_line, int amount, exexex* good_line, int width, int delta);
void creating_lines_without_external_interaction(exexex* starting_points, exexex* finishing_points, int amount_lines, int width, int delta, exexex** output);
void cosinus(exexex** ext_lines, int amount, int width, int height, int delta, float** output);
void difference_phases(float** input1, float** input2, int height, int width, int delta, int** output);