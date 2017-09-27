/*
 * test_objscan.cpp
 *
 *  Created on: 2017. 2. 25.
 *      Author: decoz
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "objscan.h"

using namespace cv;

int main(int argc, char **argv) {


	if(argc <  2) {
		printf("need filename argc[%d] \n", argc);
		return 0;
	}

	objscan scan;

	for(int i=1; i<argc; i++){

		Mat img = imread(argv[i], CV_LOAD_IMAGE_GRAYSCALE);
		if(img.empty()) {
			printf("fail to load %s\n", argv[i]);
			continue;
		}

		threshold(img,img,150,255,THRESH_BINARY);
		scan.loadImage(img);
		scan.scan(0);
		scan.scan(1);

		Mat nimg(img.size(), CV_8UC1);
		nimg.setTo(0);

		for(Iter_obj i = scan.objects.begin(); i != scan.objects.end(); i++){
			i->draw(nimg, 100, 100 , Scalar(255));
		}

		fflush(stdout);
		imshow("objscan_test src",img);
		imshow("objscan_test result",nimg);
		waitKey(0);
	}

	return 0;
}
