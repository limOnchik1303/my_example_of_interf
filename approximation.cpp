#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#pragma once
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
using namespace cv;

int rectangle(struct exexex* input, int x1, int y1, int x2, int y2) {//x1 y1 - левый нижний угол, x2 y2 - правый верхний угол
	int i = 0, res = 0;
	while (input[i].x != x1)
		i++;
	while (input[i].x < x2) {
		if ((y1 <= input[i].y) && (input[i].y < y2))
			res++;
		i++;
	}
	return res;
}

int kolvo_tochek(struct exexex centre, struct exexex* input, int h, int w, int height, int width) {
	int res = 0;
	if (centre.x < 2 * w) {		//левый край
		if (centre.y < 2 * h)	//нижний край
			res = rectangle(input, 0, 0, centre.x + w, centre.y + h);
		else if (centre.y >= height - 2 * h)	//верхний край
			res = rectangle(input, 0, centre.y - h, centre.x + w, height);
		else
			res = rectangle(input, 0, centre.y - h, centre.x + w, centre.y + h);	//середина
	}
	else if (centre.x < 2 * w) {		//правый край
		if (centre.y < 2 * h)	//нижний край
			res = rectangle(input, centre.x - w, 0, width, centre.y + h);
		else if (centre.y >= height - 2 * h)	//верхний край
			res = rectangle(input, centre.x - w, centre.y - h, width, height);
		else
			res = rectangle(input, centre.x - w, centre.y - h, width, centre.y + h);	//середина
	}
	else {                        //середина
		if (centre.y < 2 * h)	//нижний край
			res = rectangle(input, centre.x - w, 0, centre.x + w, centre.y + h);
		else if (centre.y >= height - 2 * h)	//верхний край
			res = rectangle(input, centre.x - w, centre.y - h, centre.x + w, height);
		else
			res = rectangle(input, centre.x - w, centre.y - h, centre.x + w, centre.y + h);	//середина
	}
	return res;
}

exexex min_rasto(exexex* input, int amount, exexex p, int height, int width) {
	int res = 0, min = height * height + width * width, k_min,  i = 0, t;
	while ((input[i].x != p.x + 1) && (i < amount))
		i++;
	k_min = i;
	while (i < amount) {
		t = (input[i].x - p.x) * (input[i].x - p.x) + (input[i].y - p.y) * (input[i].y - p.y);
		if (t < min) {
			min = t;
			k_min = i;
		}
		i++;
	}
	return input[k_min];
}


int selection_points(exexex* input, int amount, exexex zero, int height, int width, int epsilon, exexex* output) {
	int output_amount = 0, i = 0, min, k_min;
	exexex p = zero;
	while ((i < amount) && (output[output_amount].x < width - 1)) {
			output[output_amount] = min_rasto(input, amount, p, height, width);
			p = output[output_amount];
			output_amount++;
			i++;
		}
	return output_amount;
}