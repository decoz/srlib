/*
 * classifier.h
 *
 *  Created on: 2016. 10. 4.
 *      Author: decoz
 */

#ifndef OPENCV_LAB_CLASSIFIER_H_
#define OPENCV_LAB_CLASSIFIER_H_
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "model.h"

#define DATA_WIDTH 		20
#define DATA_HEIGHT 	30

using namespace cv;
using namespace cv::ml;

namespace srlib {

class classifier {
public:
	model_type req_model;
	std::string name;
	classifier();
	virtual ~classifier();
	virtual void 	train(Mat fmat, Mat imat) = 0;		//  case_mat �� �о�鿩 training�� ����
	virtual int	predict(Mat fmat) = 0;
	virtual void 	save(char *fname) = 0;		// 	file ��  train�� calssifier�� ����
	virtual void 	load(char *fname) = 0;		// 	file ��  train�� calssifier�� ����

};



class clsf_knn : public classifier {
private:
	Ptr<KNearest> knn;
	void initKnn();
	Mat fmat, imat;


public:
	clsf_knn(){ initKnn(); };

	void train(Mat fmat, Mat imat);		//  case_mat �� �о�鿩 training�� ����

	int	  predict(Mat fmat);
	void save(char *fname);		// 	file ��  train�� calssifier�� ����
	void load(char *fname);		// 	file ��  train�� calssifier�� ����
};




class clsf_svm : public classifier {
private:
	void initSvm();

public:
	Ptr<SVM> svm;
	clsf_svm(){ initSvm(); };

	void train(Mat fmat, Mat imat);		//  case_mat �� �о�鿩 training�� ����
	int	  predict(Mat fmat);
	void save(char *fname);		// 	file ��  train�� calssifier�� ����
	void load(char *fname);		// 	file ��  train�� calssifier�� ����
};






} /* namespace learn */

#endif /* OPENCV_LAB_CLASSIFIER_H_ */
