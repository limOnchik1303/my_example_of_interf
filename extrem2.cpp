#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

int check(long* arr, int x0, int start, int finish, int znak) {
	int flag = 0;
	if (znak == 0) {							//если знак = 0, то это для минимума
		for (int i = start; i < finish; i++) {
			if (arr[i] >= arr[x0])
				flag = 0;
			else {
				flag = 1;
				break;
			}
		}
	}
	else {										//если знак = 1, то это для максимума
		for (int i = start; i < finish; i++) {
			if (arr[i] <= arr[x0])
				flag = 0;
			else {
				flag = 1;
				break;
			}
		}
	}
	return flag;
}

int glob_min(long* arr, int length, int* out, int step) {
	int amount = 0, i = 0, flag, min, k_min = 0;
	long maximum = 0;
	for (int j = 0; j < length; j++) {
		if (maximum < arr[j])
			maximum = arr[j];
	}
//////////////////////////////////////////////////////////////////////////////////////
	while (i < length) {
		if (i < step) {
			min = maximum;
			flag = 1;
			while ((check(arr, i, 0, i + step, 0) == 0) && (i < step) && (i < length)) {
				if (arr[i] < min) {
					min = arr[i];
					k_min = i;
				}
				i++;
				flag = 0;
			}
			if (flag == 0) {
				out[amount] = k_min;
				amount++;
			}
			while ((check(arr, i, 0, i + step, 0) == 1) && (i < step) && (i < length)) {
				i++;
				flag = 1;
			}
		}
//////////////////////////////////////////////////////////////////////////////////////
		else if (i >= length - step) {
			min = maximum;
			flag = 1;
			while ((check(arr, i, i - step, length, 0) == 0) && (i >= length - step) && (i < length)) {
				if (arr[i] < min) {
					min = arr[i];
					k_min = i;
				}
				i++;
				flag = 0;
			}
			if (flag == 0) {
				out[amount] = k_min;
				amount++;
			}
			while ((check(arr, i, i - step, length, 0) == 1) && (i >= length - step) && (i < length)) {
				i++;
				flag = 1;
			}
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
		else {
			min = maximum;
			flag = 1;
			while ((check(arr, i, i - step, i + step, 0) == 0) && (step <= i) && (i < length - step) && (i < length)) {
				if (arr[i] < min) {
					min = arr[i];
					k_min = i;
				}
				i++;
				flag = 0;
			}
			if (flag == 0) {
				out[amount] = k_min;
				amount++;
			}
			while ((check(arr, i, i - step, i + step, 0) == 1) && (step <= i) && (i < length - step) && (i < length)) {
				i++;
				flag = 1;
			}
		}
	}
	return amount;
}



int glob_max(long* arr, int length, int* out, int step) {
	int amount = 0, i = 0, flag, max, k_max = 0;
	//////////////////////////////////////////////////////////////////////////////////////
	while (i < length) {
		if (i < step) {
			max = 0, flag = 1;
			while (check(arr, i, 0, i + step, 1) == 0) {
				if (arr[i] > max) {
					max = arr[i];
					k_max = i;
				}
				i++;
				flag = 0;
			}
			if (flag == 0) {
				out[amount] = k_max;
				amount++;
			}
			while (check(arr, i, 0, i + step, 1) == 1) {
				i++;
				flag = 1;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		else if (i >= length - step) {
			max = 0, flag = 1;
			while (check(arr, i, i - step, length, 1) == 0) {
				if (arr[i] > max) {
					max = arr[i];
					k_max = i;
				}
				i++;
				flag = 0;
			}
			if (flag == 0) {
				out[amount] = k_max;
				amount++;
			}
			while (check(arr, i, i - step, length, 1) == 1) {
				i++;
				flag = 1;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		else {
			max = 0;
			flag = 1;
			while (check(arr, i, i - step, i + step, 1) == 0) {
				if (arr[i] > max) {
					max = arr[i];
					k_max = i;
				}
				i++;
				flag = 0;
			}
			if (flag == 0) {
				out[amount] = k_max;
				amount++;
			}
			while (check(arr, i, i - step, i + step, 1) == 1) {
				i++;
				flag = 1;
			}
		}
	}
	return amount;
}