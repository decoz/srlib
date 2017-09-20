/*
 * test_view.cpp
 *
 *  Created on: 2017. 2. 17.
 *      Author: decoz
 */

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "cview.h"
#include "zoomview.h"

using namespace cv;
using namespace srlib;

int main( int argc, char **argv ){

	Mat big = imread("images/zoomview.png");
	zoomview::show("zview", big, Size(800,600));

	waitKey(0);
	return  0;

}



