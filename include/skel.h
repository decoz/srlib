/*
 * skel.h
 *
 *  Created on: 2014. 8. 12.
 *      Author: decoz
 *
 * ���� �̹����� skeleton ���·� ��ȯ�ϴ� Ŭ����
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
