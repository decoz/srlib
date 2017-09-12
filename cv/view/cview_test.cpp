/*
 * test_view.cpp
 *
 *  Created on: 2017. 2. 17.
 *      Author: decoz
 */

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "cview.h"

using namespace cv;
using namespace srlib;

int main( int argc, char **argv ){
	printf("start cview_test\n");


	Mat img = imread(argv[1]);
	cview::turnon("cview", "xyhistogram");
	cview::show("cview", img);

	fflush(stdout);
	waitKey(0);
	return  0;

}


