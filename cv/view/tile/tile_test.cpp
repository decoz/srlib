/*
 * test_view.cpp
 *
 *  Created on: 2017. 2. 17.
 *      Author: decoz
 */

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "tile.h"

using namespace cv;
using namespace srlib;

int main( int argc, char **argv ){
	printf("start tile_test\n");

	tile t;


	char fname[100];
	for(int i=1; i<=11; i++){
		sprintf(fname, "images/tiles/t%02d.png",i);

		destroyWindow("tile");

		Mat tileimg = imread(fname);
		printf(":%s /%dx%d \n",fname, tileimg.cols, tileimg.rows);

		t.attach(TILE_H, tileimg);

		imshow("tiles",t.mat());
		fflush(stdout);
		if( waitKey(0) == 'q') break;
	}

	imshow("tiles",t.mat());
	waitKey(0);
	return  0;

}



