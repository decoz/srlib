/*
 * dcModeller.cpp
 *
 *  Created on: 2016. 10. 4.
 *      Author: decoz
 */

#include "model.h"

namespace srlib {


Mat flat::make(Mat gimg){
	// img 로부터 model matrix 를 추출한다.

	if(gimg.channels() != 1) {
		Mat empty;
		return empty;
	}

	resize(gimg, tpl_mat, Size(mat_w,mat_h) );
	tpl_mat.convertTo(tpl_mat, CV_32FC1);
	data_mat = tpl_mat.reshape(1, 1);

	return data_mat;
}

void flat::saveConfig(FileStorage &fs) {
	fs << "mat_w" << mat_w;
	fs << "mat_h" << mat_h;
}

void flat::loadConfig(FileNode fn) {
	mat_w = fn["mat_w"];
	mat_h = fn["mat_h"];
	length  = mat_w*mat_h;

	//printf("[flat::loadconfig] %d , %d\n ", mat_w, mat_h);
}


} /* namespace fast_shape */
