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
using namespace cv;

void sort_points(exexex* input, int amount) {
	int temp;
	int* arr = (int*)malloc(amount * sizeof(int));
	for (int i = 0; i < amount; i++) {
		arr[i] = input[i].y;
	}
	for (int i = 0; i < amount - 1; i++) {
		for (int j = (amount - 1); j > i; j--) // для всех элементов после i-ого
		{
			if (arr[j - 1] > arr[j]) // если текущий элемент меньше предыдущего
			{
				int temp = arr[j - 1]; // меняем их местами
				arr[j - 1] = arr[j];
				arr[j] = temp;
			}
		}
	}
	for (int i = 0; i < amount; i++) {
		input[i].y = arr[i];
	}
}



void additon(exexex* bad_line, int amount, exexex* good_line, int width, int delta) {
	int i = 1, flag, x1, y1, x2, y2;
	float k, b;
	for (int j = 0; j < amount; j++) {
		if (bad_line[j].x >= delta) {
			x1 = bad_line[j].x;
			y1 = bad_line[j].y;
			break;
		}
	}
	for (int j = 0; j < x1; j++) {
		good_line[j].x = j;
		good_line[j].y = y1;
	}
	x2 = x1 + 1;
	while (x2 < width) {
		flag = 0;
		for (int j = 0; j < amount; j++) {
			if (x2 == bad_line[j].x) {
				y2 = bad_line[j].y;
				flag = 1;
				break;
			}
		}
		if (flag == 1) {
			k = (y2 - y1) / (x2 - x1);
			b = y1 - k * x1;
			for (int i = x1; i < x2; i++) {
				good_line[i].y = k * i + b;
				good_line[i].x = i;
			}
			x1 = x2;
			y1 = y2;
		}
		x2++;
	}
}

void creating_lines_without_external_interaction(exexex* starting_points, exexex* finishing_points, int amount_lines, int width, int delta, exexex** output) {
	float k, b;
	float y1, y2, x1, x2;
	sort_points(starting_points, amount_lines);
	sort_points(finishing_points, amount_lines);
	for (int i = 0; i < amount_lines; i++) {
		y1 = starting_points[i].y;
		y2 = finishing_points[i].y;
		x1 = starting_points[i].x;
		x2 = finishing_points[i].x;
		k = (y2 - y1) / (x2 - x1);
		b = (y1 * x2 - y2 * x1) / (x2 - x1);
		for (int j = delta; j < width - delta; j++) {
			output[i][j].x = j;
			output[i][j].y = (int)(j * k + b);
		}
	}
}

void cosinus(exexex** ext_lines, int amount, int width, int height, int delta, float** phasa) {
	float pi = 3.14, fi0, w, A, B = pi / 2, x;
	int number_line, y1, y2;
	for (int i = delta; i < width - delta; i++) {
		exexex* prom = (exexex*)malloc(2 * amount * sizeof(exexex));
		for (int j = 0; j < amount; j++) {
			prom[j] = ext_lines[j][i];
		}
		sort_points(prom, amount);
		number_line = 0;
		for (int j = 0; j < height; j++) {
			if (j < prom[0].y)
				phasa[i][j] = 0;
			else if (j >= prom[amount - 1].y)
				phasa[i][j] = (amount - 1) * pi;
			else {
				if (j == prom[number_line].y) {
					x = pi * number_line;
					number_line++;
				}
				else {
					y1 = prom[number_line - 1].y;
					y2 = prom[number_line].y;
					x = pi * (number_line - 1) + pi * (j - y1) / (y2 - y1);
				}
				phasa[i][j] = x;
			}
		}
	}
}

void difference_phases(float** input1, float** input2, int height, int width, int delta, int** output) {
	float pi = 3.14;
	int x;
	for (int i = delta; i < width - delta; i++) {
		for (int j = 0; j < height; j++) {
			x = (int)(255 * (-input1[i][j] + input2[i][j]) / (2 * pi));
			if (x > 22)
				output[i][j] = x;
			else
				output[i][j] = 0;
		}
	}
}