/*
 * classifier.cpp
 *
 *  Created on: 2016. 10. 4.
 *      Author: decoz
 */

#include "classifier.h"

namespace srlib {

classifier::classifier() {
	// TODO Auto-generated constructor stub

}

classifier::~classifier() {
	// TODO Auto-generated destructor stub
}


void clsf_svm::initSvm(){
	name = "svm";
	req_model = mt_fixed_float;

	svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR      );

	svm->setTermCriteria( TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6) );

}

void clsf_svm::train(Mat fmat, Mat imat){
	svm->train(fmat, ROW_SAMPLE, imat);
}

int clsf_svm::predict(Mat fmat){
	return (int) svm->predict(fmat);
}


void clsf_svm::save(char *fname){
// 	file 에  train된 calssifier를 저장

	/*
	FileStorage fs(fname, FileStorage::WRITE);
	svm->write(fs);
	fs.release();
	*/

	svm->save(fname);
}

void clsf_svm::load(char *fname){
// 	file 에  train된 calssifier를 저장
	//delete(svm);

	FileStorage fs(fname, FileStorage::READ);
	svm->read(fs["opencv_ml_svm"]);
	fs.release();

	//svm = SVM::load<SVM> ( fname );

}


void clsf_knn::initKnn(){
	name = "knn";
	req_model = mt_fixed_float;

	knn = KNearest::create();
	//knn->setAlgorithmType(KNearest::Types::BRUTE_FORCE);
	knn->setDefaultK(3);
}

void clsf_knn::train(Mat cases, Mat labels){
	fmat = cases, imat = labels;

	Ptr<TrainData> trainingData = TrainData::create(fmat, cv::ml::ROW_SAMPLE, imat);
	knn->train( trainingData );

	if(knn->isTrained()) knn->setIsClassifier(true);

}

int clsf_knn::predict(Mat fmat){

	return (int) knn->predict(fmat);
}


void clsf_knn::save(char *fname){

	knn->save(fname);

}

void clsf_knn::load(char *fname){

	knn = KNearest::load<KNearest>(fname);

	if(knn->isTrained()) {
		printf("trained!\n");
		knn->setIsClassifier(true);
		printf("default k : %d  \t var count: %d \n",
			knn->getDefaultK(),
			knn->getVarCount()
		);
	}


	fflush(stdout);
}







} /* namespace learn */
