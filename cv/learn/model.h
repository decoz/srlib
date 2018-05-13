/*
 * dcModeller.h
 *
 *  Created on: 2016. 10. 4.
 *      Author: decoz
 *
 *  �̹����� �����Ͽ� �н� ������ �𵨷� ��ȯ�ϴ� Ŭ����
 *	file loading ��� , img loading �� �����ϸ�
 *
 *
 */

#ifndef OPENCV_LAB_DCMODELLER_H_
#define OPENCV_LAB_DCMODELLER_H_
#include <opencv2/opencv.hpp>
#include <stdlib.h>

#define DATA_WIDTH 		20
#define DATA_HEIGHT 	30


using namespace cv;
using namespace std;

namespace srlib {

enum model_type {
	mt_fixed_float
};


class model {

public:
	int length;
	string name;
	model_type type;

	model(){};
	virtual ~model(){};
	virtual Mat	make(Mat gimg) = 0;	// img �κ��� model matrix �� �����Ѵ�.
	virtual void loadConfig(FileNode fn) = 0;
	virtual void saveConfig(FileStorage &fs) = 0;
};


class flat: public model {

	int mat_w,mat_h;
	Mat tpl_mat, data_mat ;

public:
	flat() { name = "flat_vector", type = mt_fixed_float ;} ;
	flat(int w, int h) : flat() { mat_w = w, mat_h  = h , length = w*h; };
	Mat 	make(Mat  gimg);
	void loadConfig(FileNode fn);
	void saveConfig(FileStorage &fs);
};




} /* namespace fast_shape */

#endif /* OPENCV_LAB_DCMODELLER_H_ */
