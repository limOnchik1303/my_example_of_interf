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
#include "horizontal_section.h"
#include "slice_processing.h"
using namespace cv;

void inverse_abel_transform(int** input, best_cut cut, int parameter, int* output) {
	int* intermediate_array = (int*)malloc(500 * sizeof(int));
	double* arr = (double*)malloc(500 * sizeof(double));
	int width = creating_one_srez(input, cut, intermediate_array), r0 = width / 2 + width % 2;
	double deriv, pi = 3.14, sum, y, max = 0;
	for (int r = 0; r <= r0; r++) {
		sum = 0.0;
		y = r;
		for (int i = r0 + r; i < width; i++) {
			y = r + (double)(i + 1 - r0 - r) / 2;
			deriv = derivative(input, cut, parameter, y);
			sum = sum + deriv / (pow((y * y - r * r), 0.5));
		}
		arr[r] = (double)(-1) * sum / pi;
		if (arr[r] > max)
			max = arr[r];
	}
	for (int r = 0; r <= r0; r++)
		output[r] = (int)(100 * arr[r] / max);
}