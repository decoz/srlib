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

	if(argc <  2) {
		printf("need filename argc[%d] \n", argc);
		return 0;
	}

	linescan scan;
	scan.merge_other_obj = true;
	scan.assemble_range = 100;
	scan.assemble_thresh = 0.5;

	for(int i=1; i<argc; i++){
		Mat gray = imread(argv[i], CV_LOAD_IMAGE_GRAYSCALE);
		if(gray.empty()){
			printf("error reading fjile %s\n", argv[1]);
			return 0;
		}

		threshold(gray,gray,150,255,THRESH_BINARY);
		gray = gray / 2;


		scan.scanline(gray);
		scan.DrawPaths(gray);


		imshow("objscan_test result",gray);
		fflush(stdout);;
		waitKey(0);
	}

	return 0;
}
