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

void vertical_line(IplImage* image, int* arr, int length, int r, int g, int b);
void picture(IplImage* image, long* arr, int length);
void grafik_square(long* arr, int amount_arr, int* extrem, int amount_extrem);
void draw_one_line(exexex* input, int amount, IplImage* image, int r, int g, int b, int delta);
void creating_array(int* extrem, int amount, long* input, int length, long* output, int krai);
void draw_phase_diagram(int** massiv, int width, int height, int delta, IplImage* image);
void exexex_massiv_points(exexex* input, int amount, IplImage* image, int r, int g, int b);
void draw_straight(int x, int y, float corner, IplImage* image);
void graf_phase_srez(int** input, best_cut point);
void picture2(IplImage* image, int* arr, int length, int r, int g, int b);
void draw_circle(IplImage* image, int r0, int* input);
void vert_line1(IplImage* image, int x);