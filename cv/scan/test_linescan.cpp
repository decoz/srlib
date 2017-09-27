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
	scan.debug = false;
	scan.line_max_width = 20;
	scan.line_min_length = 5;;

	scan.merge_other_obj = true;
	scan.assemble_range = 40;
	scan.adp_thresh = 3;
	scan.assemble_thresh = 0.7;

	for(int i=1; i<argc; i++){
		Mat gray = imread(argv[i], CV_LOAD_IMAGE_GRAYSCALE);
		if(gray.empty()){
			printf("error reading fjile %s\n", argv[1]);
			return 0;
		}

		imshow("gray", gray);
		threshold(gray,gray,100,255,THRESH_BINARY);
		gray = gray / 2;



		scan.scanline(gray);
		scan.DrawPaths(gray);

		imshow("objscan_test result",gray);
		fflush(stdout);;
		if(waitKey(0) == 'q') break;;
	}

	return 0;
}
