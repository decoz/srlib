/*
 * dcLearn.h
 *
 *  Created on: 2016. 1. 14.
 *      Author: decoz
 *
 *  ���� ���� Ŭ����
 *  - svm,  knn ���� �н��� ����
 *	- ��Ƽ ���� ���� (����� flat_vector ��)
 * 	- �н������ (verification) ���
 *
 */

#ifndef DCLEARN_DCLEARN_H_
#define DCLEARN_DCLEARN_H_
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include "model.h"
#include "classifier.h"


using namespace cv;
using namespace cv::ml;
using namespace std;

namespace srlib {

class learn {
private:

	classifier *clsf;
	model  	*md;

	string md_name;
	string clsf_name;


	char  *case_file, *classifier_file;

	Mat	cases, labels;							// trainning data data target class
	Mat 	vcases, vlabels, tcases, tlabels;		// verification set

	int l; 									// value dimension (decided when building)

	vector <string> lbl_names;		// ���̺� : Ŭ�����̸� ( �̸��� ���� ���̺��� �ε��� ��ȣ�� ��� )

	int 	get_label(string lbl_name, bool pushnew = true);

	void	fswrite_labelName(FileStorage &fs);
	void	fswrite_modelInfo(FileStorage &fs);
	void	fnread_labelName(FileNode fn);
	void	fnread_modelInfo(FileNode fn);




public:

	Mat	vtable; 	// verification ���� ��� label  ���� test, hit, fp (false positive ) ���� ����ȴ�.

	learn();			// �н��� ������ ����
						// ������ ���� ��� �о�� ������ �������·�


	bool feed_back(Mat f_mat, int target ); 		// training data �� �Է¹��� (���� �н����� ����)

	void 	save_cases(char *fname);					// training data �� ����
	void 	load_cases(char *fname);					// training data �� ����
	void 	save_classifier(char *fname);				// �Ǻ��� ����
	void 	load_classifier(char *fname);				// �Ǻ��� �ε�

	void 	build_classifier();

	void 	case_img(Mat img, string lbl_name);
	void 	select_model(string model_name);
	void 	select_model(model *_md) 		{ md = _md; };

	void 	select_classifier(string classifier_name);
	void 	select_classifier(classifier *_clsf)	{ clsf = _clsf; };

	string	predict_img(Mat img);	// ���� ����� ����

	void	init_verification();				// verify table �� �����ϰ� �ʱ�ȭ
	void	create_vcases(int tset_size ) ;	// �з��⸦ �׽�Ʈ�� ���� ����
	int		verify();							// verify ���� hit Ƚ���� ����
	void 	print_vtable();



	virtual ~learn();
};

} /* end of srlib */

#endif /* DCLEARN_DCLEARN_H_ */
