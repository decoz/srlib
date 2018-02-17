/*
 * util_test.cpp
 *
 *  Created on: 2017. 9. 25.
 *      Author: decoz
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "util.h"

using namespace cv;
using namespace srlib;

void test_rect();

int main(int argc, char **argv) {

	test_rect();

	waitKey(0);
	return 0;
}

void test_rect(){

	Mat m(400,400,CV_8UC3);
	m.setTo(0);
	Rect 	r[3] = { 	Rect( 100,100, 150,150 ),  Rect( 120,130, 150,150 ),
						rectsum(r[0], r[1])
					};
	for(int i=0; i<3; i++) rectangle(m, r[i], randcolor(100,150));
	imshow("test_rect",m);

}

