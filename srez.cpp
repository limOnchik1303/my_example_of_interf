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
#include "extrem1.h"
#include "extrem2.h"
#include "approximation.h"
#include "approximation_function.h"
#include "horizontal_section.h"

void vertical_line(IplImage *image, int* arr, int length, int r, int g, int b) {
	for (int i = 0; i < length - 1; i++)
		cvLine(image, cvPoint(arr[i], 0), cvPoint(arr[i], image->height), (r, g, b), 1, 8, 0);
};

void draw_one_line(exexex* input, int amount, IplImage* image, int r, int g, int b, int delta) {
	for (int i = delta + 1; i < amount - delta; i++) {
		cvLine(image, cvPoint(input[i - 1].x, input[i - 1].y), cvPoint(input[i].x, input[i].y), cvScalar(r, g, b), 1, 8, 0);
	}
}
void picture(IplImage* image, long* arr, int length) {
	for (int i = 0; i < length - 1; i++)
		cvLine(image, cvPoint(i, arr[i]), cvPoint(i + 1, arr[i + 1]), (0, 0, 0), 1, 8, 0);;
}

void grafik_square(long* arr, int amount_arr, int* extrem, int amount_extrem) {
	long* promezh = (long*)malloc(amount_arr * sizeof(long));
	for (int i = 0; i < amount_arr; i++) {
		promezh[i] = arr[i] / 100;
	}
	IplImage* graf = cvCreateImage(cvSize(amount_arr, 500), 8, 1);
	picture(graf, promezh, amount_arr);
	vertical_line(graf, extrem, amount_extrem, 0, 0, 0);
	cvNamedWindow("1", CV_WINDOW_AUTOSIZE);
	cvShowImage("1", graf);
	cvWaitKey(0);
	cvReleaseImage(&graf);
	free(promezh);
}

void creating_array(int* extrem, int amount, long* input, int length, long* output, int krai) {
	int flag; 
	for (int i = 0; i < length; i++) {
		flag = 0;
		for (int j = 0; j < amount; j++) {
			if (i == extrem[j]) {
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			output[i] = input[i];
		else
			output[i] = krai;
	}
}

void draw_phase_diagram(int** massiv, int width, int height, int delta, IplImage* image) {
	for (int i = delta; i < width - delta; i++) {
		for (int j = 0; j < height - 1; j++) {
				cvLine(image, cvPoint(i, j), cvPoint(i, j + 1), cvScalar(massiv[i][j], massiv[i][j], massiv[i][j]), 1, 8, 0);
		}
	}
}

void exexex_massiv_points(exexex* input, int amount, IplImage* image, int r, int g, int b) {
	for (int i = 0; i < amount; i++) {
		cvLine(image, cvPoint(input[i].x, input[i].y), cvPoint(input[i].x, input[i].y + 1), cvScalar(r, g, b), 1, 8, 0);
	}
}

void draw_straight(int x, int y, float corner, IplImage* image) {
	int x1, x2, y1, y2;
	float b = y - corner * x;
	x1 = 0;
	x2 = image->width;
	y1 = (int)(b);
	y2 = (int)(corner * (image->width - 1) + b);
	cvLine(image, cvPoint(x1, y1), cvPoint(x2, y2), cvScalar(250, 250, 250), 1, 8, 0);
}

void graf_phase_srez(int** input, best_cut point) {
	int t = 1, x1, x2, y1, y2, x, y;
	int* graf;
	double k, b;
	k = point.koef;
	b = point.y - point.koef * point.x;
	x1 = point.x - t;
	x2 = point.x + t;
	y1 = (int)(k * x1 + b);
	y2 = (int)(k * x2 + b);
	while ((input[x1][y1] != 0) && (input[x2][y2] != 0)) {
		t++;
		x1 = point.x - t;
		x2 = point.x + t;
		y1 = (int)(k * x1 + b);
		y2 = (int)(k * x2 + b);
	}
	graf = (int*)malloc((2 * t + 2) * sizeof(int));
	for (int i = 0; i < 2 * t + 1; i++) {
		x = x1 + i;
		y = (int)(k * x + b);
		graf[i] = input[x][y];
	}
	IplImage* image = cvCreateImage(cvSize(2 * t + 1, 255), 8, 1);
	for (int i = 1; i < 2 * t + 2; i++) {
		cvLine(image, cvPoint(i - 1, graf[i - 1]), cvPoint(i, graf[i]), cvScalar(0, 0, 0), 1, 8, 0);
	}
	cvLine(image, cvPoint(t, 0), cvPoint(t, 255), cvScalar(0, 0, 0), 1, 8, 0);
	cvNamedWindow("5", CV_WINDOW_AUTOSIZE);
	cvShowImage("5", image);
	cvWaitKey(0);
	free(graf);
}

void picture2(IplImage* image, int* arr, int length, int r, int g, int b) {
	for (int i = 0; i < length - 1; i++)
		cvLine(image, cvPoint(i, arr[i]), cvPoint(i + 1, arr[i + 1]), cvScalar(r, g, b), 1, 8, 0);;
}

void draw_circle(IplImage* image, int r0, int* input) {
	double x;
	int intens;
	for (int i = 0; i < 2 * r0; i++) {
		for (int j = 0; j < 2 * r0; j++) {
			x = (i - r0) * (i - r0) + (j - r0) * (j - r0);
			x = pow(x, 0.5);
			if (r0 - 1 < x)
				intens = 0;
			else
				intens = input[(int)x];
			cvLine(image, cvPoint(i, j), cvPoint(i, j - 1), cvScalar(intens, intens, intens), 1, 8, 0);
		}
	}
}

void vert_line1(IplImage* image, int x) {
	cvLine(image, cvPoint(x, 0), cvPoint(x, image->height), cvScalar(0, 0, 0), 1, 8, 0);
}