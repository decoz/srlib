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

	objscan scan;

	Mat gray,img = imread("images/objscan.png");


	if(img.channels()>1) cvtColor(img,gray,CV_BGR2GRAY);
	else gray = img;

	threshold(gray,gray,150,255,THRESH_BINARY);
	scan.loadImage(gray);

	//scan.scanV();
	scan.scan(0);
	scan.scan(1);

	Mat nimg(img.size(), CV_8UC1);
	nimg.setTo(0);

	for(Iter_obj i = scan.objects.begin(); i != scan.objects.end(); i++){
		//rectangle( img, i->getRect(), Scalar(0,255,255));
		i->draw(nimg, Scalar(255));
	}

	fflush(stdout);
	imshow("objscan_test src",img);
	imshow("objscan_test result",nimg);
	waitKey(0);


	return 0;
}
