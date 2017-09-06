/*
 * test_objscan.cpp
 *
 *  Created on: 2017. 2. 25.
 *      Author: decoz
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "linescan.h"

using namespace cv;
using namespace srlib;

int main(int argc, char **argv) {

	if(argc !=  2) {
		printf("need filename\n");
		return 0;
	}

	linescan scan;

	Mat gray = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if(gray.empty()){
		printf("error reading fjile %s\n", argv[1]);
		return 0;
	}

	threshold(gray,gray,150,255,THRESH_BINARY);
	gray = gray / 2;


	scan.scanline(gray);
	scan.DrawPaths(gray);

	imshow("objscan_test result",gray);

	waitKey(0);


	return 0;
}
