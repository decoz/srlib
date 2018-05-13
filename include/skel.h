/*
 * skel.h
 *
 *  Created on: 2014. 8. 12.
 *      Author: decoz
 *
 * 글자 이미지를 skeleton 형태로 변환하는 클래스
 *
 *
 */

#ifndef DCSKELETON_H_
#define DCSKELETON_H_

#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

class skel {

public:
	int max_step;
	Mat src,skelimg;
	vector <Point> cross_list,end_list,curve_list;

	Mat createSkel(Mat img);
	void zhThinning();
	bool zhIteration(int iter);
	void showStep();

	skel();
	virtual ~skel();
};


#endif /* DCSKELETON_H_ */
