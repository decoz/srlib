/*
 * dcLearn.h
 *
 *  Created on: 2016. 1. 14.
 *      Author: decoz
 *
 *  러닝 전용 클래스
 *  - svm,  knn 등의 학습을 지원
 *	- 멀티 모델을 지원 (현재는 flat_vector 만)
 * 	- 학습기검증 (verification) 기능
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

	vector <string> lbl_names;		// 레이블 : 클래스이름 ( 이름이 없는 레이블은 인덱스 번호로 출력 )

	int 	get_label(string lbl_name, bool pushnew = true);

	void	fswrite_labelName(FileStorage &fs);
	void	fswrite_modelInfo(FileStorage &fs);
	void	fnread_labelName(FileNode fn);
	void	fnread_modelInfo(FileNode fn);




public:

	Mat	vtable; 	// verification 수행 결과 label  별로 test, hit, fp (false positive ) 값이 저장된다.

	learn();			// 학습기 파일을 설정
						// 파일이 있을 경우 읽어옴 없으면 백지상태로


	bool feed_back(Mat f_mat, int target ); 		// training data 를 입력받음 (아직 학습하지 안음)

	void 	save_cases(char *fname);					// training data 를 저장
	void 	load_cases(char *fname);					// training data 를 읽음
	void 	save_classifier(char *fname);				// 판별기 저장
	void 	load_classifier(char *fname);				// 판별기 로딩

	void 	build_classifier();

	void 	case_img(Mat img, string lbl_name);
	void 	select_model(string model_name);
	void 	select_model(model *_md) 		{ md = _md; };

	void 	select_classifier(string classifier_name);
	void 	select_classifier(classifier *_clsf)	{ clsf = _clsf; };

	string	predict_img(Mat img);	// 에측 결과를 리턴

	void	init_verification();				// verify table 을 생성하고 초기화
	void	create_vcases(int tset_size ) ;	// 분류기를 테스트할 셋을 생성
	int		verify();							// verify 수행 hit 횟수를 리턴
	void 	print_vtable();



	virtual ~learn();
};

} /* end of srlib */

#endif /* DCLEARN_DCLEARN_H_ */
