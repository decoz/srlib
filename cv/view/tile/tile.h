/*
 * dcTile.h
 *
 *  Created on: 2015. 9. 22.
 *      Author: decoz
 */

#ifndef DCTILE_H_
#define DCTILE_H_

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

namespace srlib{

#define TILE_H 0x0000
#define TILE_V 0x0001

/**
 * @brief 타일이미지 생성용클래스
 * @details
 *
 * 여러이미지를 붙이기 위한 클래스로서 다음과 같은 모드를 지원한다.
 * fixw, fixh : 타일 이미지의 크기를 고정시키는 방식
 * maxw, maxh : 이미지의 전체 넓이, 크기를 고정하는 방식
 *
 * 위의 값들이 0보다 클 경우 각각 그 효과를 나타내게 된다
 *
 */

class tile {
private:
	int maxw, maxh;
	int winw, winh; // 창의 넓이 높이를 각각 또는 동시에 fix 해둠  free 인 경우 0 으로 세팅
	int fixw, fixh; 		// image 의 넓이 높이를 각각 또는 동시에 fix 해둠

	vector <Rect> rects;
	vector <Mat> imgs;
	vector <Point> joints;


	Point  	getMaxXY();
	Mat  	adjustSize(Mat img);
	Mat 	toBGR(Mat img);

	bool	available(Rect r, Point p);	// r영역을 p 영역에 접합 가능한가 확인
	bool 	intersect(Rect r1, Rect r2); // r1과 r2가 교차하는가 확인


public:
	tile();
	virtual ~tile();

	void fixSize(int w, int h);			// 크기를 fix 함 0인 경우 해당없음
	void attach(short opt, Mat img);		// 이미지의 추가 옵션
											// TILE_H		: 우측에 추가
											// TILE_V 		: 아랫쪽에 추가
	
	Mat mat();	// tile image를 생성

};
} // end of srlib
#endif /* DCTILE_H_ */
