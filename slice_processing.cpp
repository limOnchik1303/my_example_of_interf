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
using namespace cv;

int creating_one_srez(int** input, best_cut cut, int* output) {
	int x1, y1, x2, y2, amount = 1, y;
	double k, b;
	k = cut.koef;
	b = cut.y - k * cut.x;
	x1 = cut.x - amount;
	x2 = cut.x + amount;
	y1 = (int)(k * x1 + b);
	y2 = (int)(k * x2 + b);
	while ((input[x1][y1] != 0) && (input[x2][y2] != 0) && (amount < 40)) {
		amount++;
		x1 = cut.x - amount;
		x2 = cut.x + amount;
		y1 = (int)(k * x1 + b);
		y2 = (int)(k * x2 + b);
	}
	for (int i = x1 + 1; i < x2; i++) {
		y = (int)(k * i + b);
		output[i - x1] = input[i][y];
	}
	output[x2 - x1] = 0;
	return 2 * amount + 1;
}

void coefficients_cubic_function(int* y, int N, int h, double* a, double* b, double* c, double* d, int delta) {
	double* f = (double*)malloc((N + 1) * sizeof(double));
	double* alfa = (double*)malloc((N + 1) * sizeof(double));
	double* beta = (double*)malloc((N + 1) * sizeof(double));
	int hx;
	alfa[1] = 0;
	beta[1] = 0;
	for (int i = 1; i < N; i++) {
		if ((delta != 0) && (i == N - 1))
			hx = delta;
		else
			hx = h;
		f[i] = (double)3 * (y[i - 1] - 2 * y[i] + y[i + 1]) / (hx * hx);
		if (i != 1) {
			alfa[i] = (double)(-1) / (alfa[i - 1] + 4);
			beta[i] = (double)(f[i - 1] - beta[i - 1]) / (alfa[i - 1] + 4);
		}
	}
	if (delta != 0) {
		b[N - 1] = (double)(f[N - 1] - beta[N - 1]) / (alfa[N - 1] + 4);
		a[N - 1] = (double)b[N - 1] / (-3 * delta);
		c[N - 1] = (double)(y[N] - y[N - 1]) / delta - (double)b[N - 1] * delta - (double)a[N - 1] * delta * delta;
		d[N - 1] = y[N - 1];
	}
	else {
		b[N - 1] = (double)(f[N - 1] - beta[N - 1]) / (alfa[N - 1] + 4);
		a[N - 1] = (double)b[N - 1] / (-3 * h);
		c[N - 1] = (double)(y[N] - y[N - 1]) / h - (double)b[N - 1] * h - (double)a[N - 1] * h * h;
		d[N - 1] = y[N - 1];
	}
	for (int i = N - 2; i >= 0; i--) {
		b[i] = alfa[i + 1] * b[i + 1] + beta[i + 1];
		a[i] = (double)(b[i + 1] - b[i]) / (3 * h);
		c[i] = (double)(y[i + 1] - y[i]) / h - (double)b[i] * h - (double)a[i] * h * h;
		d[i] = y[i];
	}
	free(f);
	free(alfa);
	free(beta);
}

int symmetric_spline_approximation(int** input, best_cut cut, int* output, int parameter) {
	int* intermediate_array = (int*)malloc(500 * sizeof(int));
	int width = creating_one_srez(input, cut, intermediate_array), h = width / 2 / parameter, x1, x2, number = 0, delta = (width / 2) % parameter, xxx, yyy;
	int n;
	n = parameter;
	double* a = (double*)malloc((n + 1) * sizeof(double));
	double* b = (double*)malloc((n + 1) * sizeof(double));
	double* c = (double*)malloc((n + 1) * sizeof(double));
	double* d = (double*)malloc((n + 1) * sizeof(double));
	int* y = (int*)malloc((n + 2) * sizeof(int));
	for (int i = 0; i <= n; i++)
		y[i] = intermediate_array[(width / 2) + i * h];
	if (delta != 0) {
		y[n + 1] = intermediate_array[width - 1];
		n++;
	}
	coefficients_cubic_function(y, n, h, a, b, c, d, delta);
	x2 = width / 2 + h;
	x1 = width / 2;
	output[0] = 0;
	for (int i = width / 2; i < width; i++) {
		if (i < x2) {
			xxx = (int)(a[number] * (i - x1) * (i - x1) * (i - x1) + b[number] * (i - x1) * (i - x1) + c[number] * (i - x1) + d[number]);
			yyy = intermediate_array[i];
			output[i] = xxx;
			output[width - i] = output[i];
		}
		else if (i == x2) {
			number++;
			x2 = x2 + h;
			x1 = x1 + h;
			xxx = (int)(a[number] * (i - x1) * (i - x1) * (i - x1) + b[number] * (i - x1) * (i - x1) + c[number] * (i - x1) + d[number]);
			yyy = intermediate_array[i];
			output[i] = xxx;
			output[width - i] = output[i];
		}
	}
	free(intermediate_array);
	free(a);
	free(b);
	free(c);
	free(d);
	free(y);
	return width;
}

double derivative(int** input, best_cut cut, int parameter, int point) {
	int* intermediate_array = (int*)malloc(500 * sizeof(int));
	int width = creating_one_srez(input, cut, intermediate_array), h = width / 2 / parameter, x1, x2, number = 0, delta = (width / 2) % parameter, xxx, yyy;
	int n = parameter, k = 0;
	double deriv;
	double* a = (double*)malloc((n + 1) * sizeof(double));
	double* b = (double*)malloc((n + 1) * sizeof(double));
	double* c = (double*)malloc((n + 1) * sizeof(double));
	double* d = (double*)malloc((n + 1) * sizeof(double));
	int* y = (int*)malloc((n + 2) * sizeof(int));
	for (int i = 0; i <= n; i++)
		y[i] = intermediate_array[(width / 2) + i * h];
	if (delta != 0) {
		y[n + 1] = intermediate_array[width - 1];
		n++;
	}
	coefficients_cubic_function(y, n, h, a, b, c, d, delta);
	while ((point < d[k]) && (d[k + 1] < point))
		k++;
	deriv = (double)3 * a[k] * (point - d[k]) * (point - d[k]) + (double)2 * b[k] * (point - d[k]) + c[k];
	return deriv;
}