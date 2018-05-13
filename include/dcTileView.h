/*
 * dcTileView.h
 *
 *  Created on: 2015. 9. 21.
 *      Author: decoz
 *
 *  여러 이미지를 하나의 창으로 붙여서 보여주기 위한 간단한 클래스
 *
 *	attach  		: 자동(?) .. 나중에 구현하자
 *  attachDown 	: 아랫쪽 좌측 붙임
 *	attachRight   : 우측 상단에 붙임
 *
 */

#ifndef DCTILEVIEW_H_
#define DCTILEVIEW_H_

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


class dcTileView {
private:
	int maxw, maxh; 	// 창의 넓이 높이를 각각 또는 동시에 fix 해둠  free 인 경우 0 으로 세팅
	int fixw, fixh; 		// image 의 넓이 높이를 각각 또는 동시에 fix 해둠

	vector <Rect> rects;
	vector <Mat> imgs;


	Point  getMaxXY();
	Mat  	adjustSize(Mat img);
	Mat 	toBGR(Mat img);


public:
	dcTileView();
	virtual ~dcTileView();

	void fixSize(int w, int h);		// 크기를 fix 함 0인 경우 해당없음
	void attachDown(Mat img);	// 아랫쪽에 붙임
	void attachRight(Mat img);	// 우측에 붙임
	void attach(Mat img);	// 아무대나.. 자동..( 나중에 천천히 개발)

	void show(string wname);


};

#endif /* DCTILEVIEW_H_ */
