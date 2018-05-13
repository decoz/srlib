/*
 * learn.cpp
 *
 *  Created on: 2016. 1. 14.
 *      Author: decoz
 */

#include "learn.h"
#include <stdio.h>
#include <string>
#include	"rnd.h"

using namespace cv;
using namespace cv::ml;

namespace srlib {

learn::~learn() {
	// TODO Auto-generated destructor stub
}

learn::learn(){

}

void 	learn::save_cases(char *fname){
// training data 를 저장

	printf("writing cases \n");
	fflush(stdout);
	FileStorage fs(fname, FileStorage::WRITE);

	fs << "case" << cases;
	fs << "label"	<< labels;

	fswrite_labelName(fs);
	fswrite_modelInfo(fs);

	fs.release();

}



void 	learn::load_cases(char *fname){
// training data 를 저장

	FileStorage fs(fname, FileStorage::READ);

	fs["case"] >> cases;
	fs["label"]	>> labels;

	lbl_names.clear();
	fnread_labelName(fs["label_name"]);
	fnread_modelInfo(fs[""]);

	printf("loaded cases from %s\n", fname);
	fs.release();
}




bool learn::feed_back(Mat f_mat, int target ){

	return false;
}

void 	learn::case_img(Mat img, string lbl_name){

	int label = get_label(lbl_name);

	if(img.channels() > 1) cvtColor(img,img,CV_BGR2GRAY);

	Mat fmat = md->make(img);

	cases.push_back(fmat);
	labels.push_back(label);

}

int	learn::get_label(string lbl_name, bool pushnew ){
/*
 * label matrix 를 돌며 해당 레이블을 찾아서 인덱스릴 리턴한다.
 * pushnew 가 true 이면 없는 레이블은 새로 생성한다.
 */

	for(int i=0;i<lbl_names.size();i++)
		if( lbl_name == lbl_names[i] ) return i;

	if(pushnew) {
		lbl_names.push_back(lbl_name);
		return lbl_names.size() -  1;
	} else 	return  - 1;
}



void learn::build_classifier(){

	clsf->train(cases, labels);
}

string	 learn::predict_img(Mat img){

	if(img.channels() > 1) cvtColor(img,img,CV_BGR2GRAY);

	int label = clsf->predict( md->make(img) );

	if(lbl_names.size() > label)	return lbl_names[label];
	else {
		char buff[10];
		sprintf(buff,"%d",label);
		return string(buff);
	}
}


void 	learn::save_classifier(char *fname){

	clsf->save(fname);

	FileStorage fs(fname, FileStorage::APPEND);
	fswrite_labelName(fs);
	fs << "classifier_name" << clsf->name;
	fswrite_modelInfo(fs);
	fs.release();

}


void 	learn::load_classifier(char *fname){

	clsf = NULL;

	FileStorage fs(fname, FileStorage::READ);
	lbl_names.clear();
	fnread_labelName( fs["label_name"]);
	fnread_modelInfo( fs[""]);
	string name =  fs["classifier_name"];
	fs.release();

	select_classifier(name);
	if(clsf != NULL) clsf->load(fname);

	printf("loaded %s as classifier \n", fname);
}


void	learn::fswrite_labelName(FileStorage &fs ){
// label 이름을 저장
	fs << "label_name" << "[";
	for(int i=0;i<lbl_names.size();i++)	fs <<  lbl_names[i] ;
	fs << "]";
}

void	learn::fswrite_modelInfo(FileStorage &fs ){
// label 이름을 저장

	fs << "model_name" << md->name;
	fs << "model_config" << "{";
	md->saveConfig(fs);
	fs << "}";
}

void	learn::fnread_modelInfo(FileNode fn){

	select_model(fn["model_name"]);
	md->loadConfig(fn["model_config"]);

}

void	learn::fnread_labelName(FileNode fn){
	printf("load labels [");
	for(FileNodeIterator i = fn.begin(); i != fn.end(); ++i){
		string s = (string) *i;
		lbl_names.push_back(s);
		printf("%s ",s.c_str());
	}
	printf("]\n");
}

void 	learn::select_model(string model_name){
	md_name = model_name;
	if( model_name == "flat_vector" ) 	md = new flat();
};

void 	learn::select_classifier(string classifierl_name){
	clsf_name = classifierl_name;
	if(clsf_name ==  "svm" ) clsf =  new clsf_svm();
	if(clsf_name ==  "knn" ) clsf = new clsf_knn();
};


void	learn::create_vcases(int tset_size ){
	int cs = cases.rows;
	if( tset_size >= cs ) {
		printf("error: tset size is too big! \n");
		return ;
	}

	int *tlist = rnd::nrand(cs, tset_size );

	vcases = Mat(0,cases.cols, cases.type());
	vlabels = Mat(0,1, labels.type());

	tcases = Mat(0,cases.cols, cases.type());
	tlabels = Mat(0,1, labels.type());

	for(int i=0;i<cs;i++){
		bool ist = false;
		for(int t=0; t<tset_size; t++)
			if( tlist[t] == i ) {
				ist = true;
				tcases.push_back(cases.row(i));
				tlabels.push_back(labels.row(i));
				break;
			}
		if(!ist) {
			vcases.push_back(cases.row(i));
			vlabels.push_back(labels.row(i));
		}
	}

}

void	learn::init_verification(){
/*
 * @breif verify table 을 생성하고 초기화
 */
	int n = lbl_names.size();
	vtable = Mat(n,3,CV_16U);
	vtable.setTo(0);
}


int	learn::verify(){
/*
 * @brief verification process 를 수행
 */
	if( clsf == NULL ){
		printf("error: select classifier first");
		return -1;
	}

	int hit_cnt = 0;
	clsf->train(vcases, vlabels);

	for(int i=0;i<tcases.rows; i++){
		int r =  clsf->predict( tcases.row(i));
		int i_lbl = tlabels.at<int>(i,0);

		vtable.at<unsigned short>(i_lbl, 0)++;

		if(r == i_lbl ) {
			hit_cnt++;
			vtable.at<unsigned short>(i_lbl, 1)++;
		} else {
			vtable.at<unsigned short>(r, 2)++;
		}
	}

	return hit_cnt;
}

void 	learn::print_vtable(){

	for(int i=0; i<vtable.rows ; i++){

		unsigned short
		t = vtable.at<unsigned short>(i,0),
		h = vtable.at<unsigned short>(i,1),
		fp = vtable.at<unsigned short>(i,2);

		printf("%4.8s  T:%3d \th:%3d \tm:%3d \tfp:%3d",
				lbl_names[i].c_str(), t, h, t-h, fp);
		if(t > 0) printf("\t%3d%% \n", h * 100 / t);
		else printf("\n");
	}
}

}
