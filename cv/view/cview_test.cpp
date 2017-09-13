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

	for(int i=1;i<argc;i++){
		Mat img = imread(argv[1] /*,CV_LOAD_IMAGE_GRAYSCALE */);
		cview::turnon("cview", "xyhistogram");
		cview::show("cview", img);

		fflush(stdout);
		if( waitKey(0) == 'q') break;
	}
	return  0;

}


