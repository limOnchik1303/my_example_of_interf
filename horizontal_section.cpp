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

int axis_gravity (int** input, int width, int height, int delta, exexex* output) {
	int mass, centre_mass, res = 0;
	for (int i = 0; i < height; i++) {
		mass = 0;
		centre_mass = 0;
		for (int j = delta; j < width - delta; j++) {
			mass = mass + input[j][i];
			centre_mass = centre_mass + j * input[j][i];
		}
		if (mass >= 1000) {
			output[res].x = centre_mass / mass;
			output[res].y = i;
			res++;
		}
	}
	return res;
}

best_cut search_best_corner (int** input, exexex point, double edge, double step, int* flag) {
	best_cut res;
	double pi = 3.14, k, b, min = 1000, diff, k_min = 0, k_t = 0;
	int x1, x2, y1, y2, t;
	for (double i = -edge; i < edge; i = i + step) {
		k = tan(pi * i / 180);
		b = point.y - k * point.x;
		t = 1;
		x1 = point.x - t;
		x2 = point.x + t;
		y1 = (int)(k * x1 + b);
		y2 = (int)(k * x2 + b);
		diff = 0;
		while ((input[x1][y1] != 0) && (input[x2][y2] != 0)) {
			diff = diff + abs(input[x1][y1] - input[x2][y2]);
			t++;
			x1 = point.x - t;
			x2 = point.x + t;
			y1 = (int)(k * x1 + b);
			y2 = (int)(k * x2 + b);
		}
		diff = diff / t;
		if (diff < min) {
			min = diff;
			k_min = k;
			k_t = t;
		}
	}
	if ((30 <= k_t) && (k_t <= 40))
		*flag = 0;
	else
		*flag = 1;
	res.x = point.x;
	res.y = point.y;
	res.dif = min;
	res.koef = k_min;
	return res;
}

void search_best_section_x(int** input, exexex* centre, int amount, double edge, double step, best_cut* output, int amount_points) {
	best_cut prom, res;
	double min = 100;
	int flag, k_y, x_flag = 0;
	for (int k = 0; k < amount_points; k++) {
		output[k].x = 0;
		output[k].y = 0;
		output[k].koef = 0;
		output[k].dif= 1000;
	}
	for (int k = 0; k < amount_points; k++) {
		min = 1000;
		for (int i = 0; i < amount; i++) {
			flag = 0;
			for (int j = 0; j < amount_points; j++) {
				if (output[j].y == centre[i].y) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				prom = search_best_corner(input, centre[i], edge, step, &x_flag);
				if ((x_flag == 0) && (min > prom.dif)) {
					min = prom.dif;
					res = prom;
				}
			}
		}
		output[k] = res;
	}
}