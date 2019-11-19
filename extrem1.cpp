#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

int func_max(long* arr, int length, int* ext, int step) {									//создает массив экстремумов
	int amount = 0, maximum;

	int number = length / step, k_ext;

	for (int j = 0; j < number; j++) {
		maximum = 0;
		for (int i = j * step; i < (j + 1) * step; i++) {
			if (arr[i] > maximum) {
				maximum = arr[i];
				k_ext = i;
			}
		}
		ext[amount] = k_ext;
		amount++;
	}
	maximum = 0;
	for (int i = number * step; i < length; i++) {
		if (arr[i] > maximum) {
			maximum = arr[i];
			k_ext = i;
		}
	}
	ext[amount] = k_ext;
	amount++;
	return amount;
}

int func_min(long* arr, int length, int* ext, int step) {									//создает массив экстремумов
	int amount = 0, maximum = arr[0], minimum;

	for (int n = 0; n < length; n++) {
		if (maximum < arr[n])
			maximum = arr[n];
	}

	int number = length / step, k_ext;

	for (int j = 0; j < number; j++) {
		minimum = maximum;
		for (int i = j * step; i < (j + 1) * step; i++) {
			if (arr[i] < minimum) {
				minimum = arr[i];
				k_ext = i;
			}
		}
		ext[amount] = k_ext;
		amount++;
	}
	minimum = maximum;
	for (int i = number * step; i < length; i++) {
		if (arr[i] < minimum) {
			minimum = arr[i];
			k_ext = i;
		}
	}
	ext[amount] = k_ext;
	amount++;
	return amount;
}

long standard_deviation(long* arr, int length, int x0, int step) {									//создает массив экстремумов
																									//возвращает количество элементов в массиве
	long  A = 0, B = 0, C = 0, res = 0;
	int y0 = arr[x0];

	if ((x0 >= step) && (x0 < length - step)) {
		for (int i = x0 - step; i <= x0 + step; i++) {
			A = A + (arr[i] - y0) * i * i;
			B = B + i * i * i * i;
			C = C + (arr[i] - y0) * (arr[i] - y0);
		}
	}
	else if (x0 < step) {
		for (int i = 0; i < x0 + step; i++) {
			A = A + (arr[i] - y0) * i * i;
			B = B + i * i * i * i;
			C = C + (arr[i] - y0) * (arr[i] - y0);
		}
	}
	else {
		for (int i = length - step; i < length; i++) {
			A = A + (arr[i] - y0) * i * i;
			B = B + i * i * i * i;
			C = C + (arr[i] - y0) * (arr[i] - y0);
		}
	}

	res = A * A / B;
	res = C - res;
	return res;
}

void averaging(long* input, int length, int step, long* output) {
	int aver;
	for (int i = 0; i < length; i++) {
		aver = 0;
		if (i < step) {
			for (int j = 0; j < i + step; j++)
				aver = aver + input[j];
			output[i] = aver / (i + step);
		}
		else if (i >= length - step) {
			for (int j = i - step; j < length; j++)
				aver = aver + input[j];
			output[i] = aver / (length - i + step);
		}
		else {
			for (int j = i - step; j < i + step; j++)
				aver = aver + input[j];
			output[i] = aver / (2 * step);
		}
	}
}