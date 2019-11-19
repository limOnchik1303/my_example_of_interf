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
#include "abel.h"
using namespace cv;

IplImage* image = 0;
IplImage* src = 0;

void draw_extrem(int* arr, int amount, IplImage* image, int x) {
	for (int i = 0; i < amount; i++) {
		cvLine(image, cvPoint(x, arr[i]), cvPoint(x, arr[i] + 1), (250, 250, 250), 1, 8, 0);
	}
}

int main(int argc, char* argv[]) {

	char* filename = argc == 2 ? argv[1] : "D:\example4.tif";				// имя картинки задаётся первым параметром

	IplImage* image = cvLoadImage(filename, 1);
	IplImage* result = cvLoadImage(filename, 1);
	int av_step = 20;
	int step_min_d = 10;
	int step_min1 = 40;
	int step_min2 = 50;
	int step_min3 = 90;
	int step_max_d = 27;
	int step_max1 = 10;
	int step_max2 = 10;
	int step_max3 = 88;

	long* curve;
	long* square_min;
	long* square_max;
	
	int h, kolvo_min = 0, kolvo_max = 0, maximum = 0;

	struct exexex* massiv_min = (struct exexex*)malloc(image->width * 100 * sizeof(exexex));
	struct exexex* massiv_max = (struct exexex*)malloc(image->width * 100 * sizeof(exexex));

	//scanf("%d", &h);
	for (int h = 0; h < image->width; h++) {
		//IplImage* graf = cvCreateImage(cvSize(image->height, 256), 8, 1);//создает массив всех точек графика (с усредненными значениями)
		curve = (long*)malloc(image->height * sizeof(long));
		long* true_curve = (long*)malloc(image->height * sizeof(long));
		for (int i = 0; i < image->height; i++)
			curve[i] = CV_IMAGE_ELEM(image, uchar, i, h * image->nChannels + 1);
		averaging(curve, image->height, av_step, true_curve);
///////////////////////////////////////////////////////////////////////////////////////////	
		long* square_min = (long*)malloc(image->height * sizeof(int));
		int* ext_min = (int*)malloc(100 * sizeof(int));
		long* arr_min1 = (long*)malloc(image->height * sizeof(long));
		long* arr_min2 = (long*)malloc(image->height * sizeof(long));
		int* x_min1 = (int*)malloc(100 * sizeof(int));
		int* x_min2 = (int*)malloc(100 * sizeof(int));

		int amount_ext_min, minmin1, minmin2, flag_min;

		for (int i = 0; i < image->height; i++)
			square_min[i] = standard_deviation(true_curve, image->height, i, step_min_d);
		amount_ext_min = glob_min(square_min, image->height, ext_min, step_min1);

		creating_array(ext_min, amount_ext_min, true_curve, image->height, arr_min1, 255);
		minmin1 = glob_min(arr_min1, image->height, x_min1, step_min2);
		creating_array(x_min1, minmin1, true_curve, image->height, arr_min2, 255);
		minmin2 = glob_min(arr_min2, image->height, x_min2, step_min3);
		//draw_extrem(x_min2, minmin2, result, h);

		for (int i = 0; i < minmin2; i++) {
			massiv_min[kolvo_min + i].x = h;
			massiv_min[kolvo_min + i].y = x_min2[i];
		}
		kolvo_min = kolvo_min + minmin2;
///////////////////////////////////////////////////////////////////////////////////
		long* square_max = (long*)malloc(image->height * sizeof(int));
		int* ext_max = (int*)malloc(100 * sizeof(int));
		long* arr_max1 = (long*)malloc(image->height * sizeof(long));
		long* arr_max2 = (long*)malloc(image->height * sizeof(long));
		int* x_max1 = (int*)malloc(100 * sizeof(int));
		int* x_max2 = (int*)malloc(100 * sizeof(int));

		int amount_ext_max, maxmax1, maxmax2, flag_max;

		for (int i = 0; i < image->height; i++)
			square_max[i] = standard_deviation(true_curve, image->height, i, step_max_d);
		amount_ext_max = glob_min(square_max, image->height, ext_max, step_max1);

		creating_array(ext_max, amount_ext_max, true_curve, image->height, arr_max1, 0);
		maxmax1 = glob_max(arr_max1, image->height, x_max1, step_max2);
		creating_array(x_max1, maxmax1, true_curve, image->height, arr_max2, 0);
		maxmax2 = glob_max(arr_max2, image->height, x_max2, step_max3);
		//draw_extrem(x_max2, maxmax2, result, h);

		for (int i = 0; i < maxmax2; i++) {
			massiv_max[kolvo_max + i].x = h;
			massiv_max[kolvo_max + i].y = x_max2[i];
		}
		kolvo_max = kolvo_max + maxmax2;

		free(curve);
		free(true_curve);
		free(square_min);
		free(ext_min);
		free(arr_min1);
		free(arr_min2);
		free(x_min1);
		free(x_min2);

		free(square_max);
		free(ext_max);
		free(arr_max1);
		free(arr_max2);
		free(x_max1);
		free(x_max2);
	}
	int epsilon = 20, delta_min = 40, delta_max = 40, i = 0, kol_max, kol_min, j, h_min = 150, delta = 10;
	exexex* line;
	exexex** massiv_lines = (exexex**)malloc(image->height * image->width * sizeof(exexex));;
	int amount_lines = 0;

	while (massiv_max[i].x != delta)
		i++;
	j = i;
	while (massiv_max[i].x == delta) {
		if (massiv_max[i].y < image->height - h_min) {
			if (i == j) {
				line = (exexex*)malloc(1000 * image->width * sizeof(exexex));
				kol_max = selection_points(massiv_max, kolvo_max, massiv_max[0], image->height, image->width, epsilon, line);
				massiv_lines[amount_lines] = (exexex*)malloc(image->width * sizeof(exexex));
				additon(line, kol_max, massiv_lines[amount_lines], image->width, delta);
				draw_one_line(massiv_lines[amount_lines], image->width, result, 100, 0, 0, delta);
				amount_lines++;
				free(line);
			}
			else if ((abs(massiv_max[i - 1].y - massiv_max[i].y) >= delta_max) && (massiv_max[i].x == delta)) {
				line = (exexex*)malloc(1000 * image->width * sizeof(exexex));
				kol_max = selection_points(massiv_max, kolvo_max, massiv_max[i], image->height, image->width, epsilon, line);
				massiv_lines[amount_lines] = (exexex*)malloc(image->width * sizeof(exexex));
				additon(line, kol_max, massiv_lines[amount_lines], image->width, delta);
				draw_one_line(massiv_lines[amount_lines], image->width, result, 100, 0, 0, delta);
				amount_lines++;
				free(line);
			}
		}
		i++;
	}

	i = 0;
	while (massiv_min[i].x != delta)
		i++;
	j = i;
	while (massiv_min[i].x == delta) {
		if (massiv_min[i].y < image->height - h_min) {
			if (i == j) {
				line = (exexex*)malloc(10000 * image->width * sizeof(exexex));
				kol_min = selection_points(massiv_min, kolvo_min, massiv_min[0], image->height, image->width, epsilon, line);
				massiv_lines[amount_lines] = (exexex*)malloc(image->width * sizeof(exexex));
				additon(line, kol_min, massiv_lines[amount_lines], image->width, delta);
				draw_one_line(massiv_lines[amount_lines], image->width, result, 250, 250, 250, delta);
				amount_lines++;
				free(line);
			}
			else if ((abs(massiv_min[i - 1].y - massiv_min[i].y) >= delta_min) && (massiv_min[i].x == delta)) {
				line = (exexex*)malloc(10000 * image->width * sizeof(exexex));
				kol_min = selection_points(massiv_min, kolvo_min, massiv_min[i], image->height, image->width, epsilon, line);
				massiv_lines[amount_lines] = (exexex*)malloc(image->width * sizeof(exexex));
				additon(line, kol_min, massiv_lines[amount_lines], image->width, delta);
				draw_one_line(massiv_lines[amount_lines], image->width, result, 250, 250, 250, delta);
				amount_lines++;
				free(line);
			}
		}
		i++;
	}

	exexex* starting_points = (exexex*)malloc(amount_lines * sizeof(exexex));
	exexex* finishing_points = (exexex*)malloc(amount_lines * sizeof(exexex));
	int st;
	for (int i = 0; i < amount_lines; i++) {
		st = 0;
		for (int j = delta; j < 3 * delta; j++) {
			st = st + massiv_lines[i][j].y;
		}
		starting_points[i].y = st / (2 * delta);
		starting_points[i].x = delta;
		st = 0;
		for (int j = image->width - 3 * delta; j < image->width - delta; j++) {
			st = st + massiv_lines[i][j].y;
		}
		finishing_points[i].y = st / (2 * delta);
		finishing_points[i].x = image->width - delta;
	}

	exexex** massiv_lines_without_external_interaction = (exexex**)malloc(image->height * image->width * sizeof(exexex));
	for (int i = 0; i < amount_lines; i++)
		massiv_lines_without_external_interaction[i] = (exexex*)malloc(image->width * sizeof(exexex));
	creating_lines_without_external_interaction(starting_points, finishing_points, amount_lines, image->width, delta, massiv_lines_without_external_interaction);

	for (int i = 0; i < amount_lines; i++) {
		draw_one_line(massiv_lines_without_external_interaction[i], image->width, result, 255, 255, 255, delta);
	}

	float** initial_massiv = (float**)malloc(image->height * image->width * sizeof(float));
	float** final_massiv = (float**)malloc(image->height * image->width * sizeof(float));
	int** out_phase = (int**)malloc(image->height * image->width * sizeof(int));
	for (int i = 0; i < image->width; i++) {
		initial_massiv[i] = (float*)malloc(image->height * sizeof(float));
		final_massiv[i] = (float*)malloc(image->height * sizeof(float));
		out_phase[i] = (int*)malloc(image->height * sizeof(int));
	}
	
	/*for (int i = 0; i < amount_lines; i++) {
		printf("%d - %d\n", massiv_lines[i][30].y, massiv_lines_without_external_interaction[i][30].y);
	}*/
	cosinus(massiv_lines_without_external_interaction, amount_lines, image->width, image->height, delta, initial_massiv);
	cosinus(massiv_lines, amount_lines, image->width, image->height, delta, final_massiv);
	/*for (int i = image->height - h_min; i > 700; i--) {
		if (initial_massiv[300][i] == final_massiv[300][i])
			printf("%f - %d\n", initial_massiv[300][i], i);
	}*/
	difference_phases(initial_massiv, final_massiv, image->height, image->width, delta, out_phase);

	IplImage* phase_diagram = cvCreateImage(cvSize(image->width, image->height), 8, 1);
	draw_phase_diagram(out_phase, image->width, image->height, delta, phase_diagram);

	exexex* centre_mass_arr = (exexex*)malloc(phase_diagram->height * sizeof(exexex));
	int amount_centre = axis_gravity(out_phase, phase_diagram->width, phase_diagram->height, delta, centre_mass_arr);
	exexex_massiv_points(centre_mass_arr, phase_diagram->height, phase_diagram, 255, 255, 255);

	best_cut* cuts = (best_cut*)malloc(4 * sizeof(best_cut));
	int nnn = 3, ttt;
	double edge = 20, step = 0.1;
	search_best_section_x(out_phase, centre_mass_arr, amount_centre, edge, step, cuts, nnn);
	/* (int i = 0; i < nnn; i++) {
		draw_straight(cuts[i].x, cuts[i].y, cuts[i].koef, phase_diagram);
		printf("%d %d %.3f\n", cuts[i].x, cuts[i].y, cuts[i].koef);
	}*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int x_flag = 0;
	best_cut cut = search_best_corner(out_phase, centre_mass_arr[200], edge, step, &x_flag);
	int* srez = (int*)malloc(phase_diagram->width * sizeof(int));
	int parameter = 9, width, max = 0;

	width = creating_one_srez(out_phase, cut, srez);
	IplImage* app_srez = cvCreateImage(cvSize(width, 100), 8, 1);
	for (int i = 0; i <= width; i++) {
		if (max < srez[i])
			max = srez[i];
	}
	for (int i = 0; i <= width; i++) {
		srez[i] = 100 - 100 * srez[i] / max;
	}
	max = 0;
	picture2(app_srez, srez, width, 255, 255, 0);
	width = symmetric_spline_approximation(out_phase, cut, srez, parameter);
	for (int i = 0; i < width; i++) {
		if (max < srez[i])
			max = srez[i];
	}
	for (int i = 0; i < width; i++) {
		srez[i] = 100 - 100 * srez[i] / max;
	}

	picture2(app_srez, srez, width, 0, 255, 0);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IplImage* density = cvCreateImage(cvSize(width, width), 8, 0);
	int* abel_den = (int*)malloc((width + 1) * sizeof(int));
	inverse_abel_transform(out_phase, cut, parameter, abel_den);
	for (int i = width / 2; i <= width; i++) {
		abel_den[i] = abel_den[width - i];
	}
	draw_circle(density, width / 2, abel_den);
	IplImage* srez_abel = cvCreateImage(cvSize(width + 1, 100), 8, 0);
	picture2(srez_abel, abel_den, width + 1, 0, 255, 0);
	vert_line1(srez_abel, width / 2);
	printf("%d", width);
	/*for (int i = 0; i < width / 2; i++)
		printf("%d %d\n", i, abel_den[i]);*/

	free(abel_den);
	free(massiv_lines);
	free(starting_points);
	free(finishing_points);
	free(massiv_lines_without_external_interaction);
	free(initial_massiv);
	free(final_massiv);
	free(out_phase);
	free(centre_mass_arr);
	free(srez);
	cvNamedWindow("3", 0);
	cvNamedWindow("4", 0);
	cvNamedWindow("5", 0);
	cvNamedWindow("6", 0);
	cvNamedWindow("7", 0);
	cvShowImage("3", result);
	cvShowImage("4", phase_diagram);
	cvShowImage("5", app_srez);
	cvShowImage("6", density);
	cvShowImage("7", srez_abel);
	cvWaitKey(0);
	cv::Mat m = cv::cvarrToMat(result, true);
	cv::imwrite("test1.tif", m);
	cv::Mat mn = cv::cvarrToMat(phase_diagram, true);
	cv::imwrite("test2.tif", mn);
	cv::Mat m1 = cv::cvarrToMat(app_srez, true);
	cv::imwrite("test3.tif", m1);
	cv::Mat m2 = cv::cvarrToMat(density, true);
	cv::imwrite("test4.tif", m2);
	cv::Mat m3 = cv::cvarrToMat(srez_abel, true);
	cv::imwrite("test5.tif", m3);

	cvWaitKey(0);									// ждём нажатия клавиши

	cvReleaseImage(&image);							// освобождаем ресурсы
	cvReleaseImage(&result);
	cvReleaseImage(&phase_diagram);
	cvReleaseImage(&app_srez);
	cvReleaseImage(&density);
	cvReleaseImage(&srez_abel);
	//cvReleaseImage(&graf);

	cvDestroyWindow("original");					// удаляем окно
	return 0;
}