/*
 * objscan.h
 *
 *  Created on: 2015. 10. 7.
 *      Author: decoz
 *
 *  ocr div 의 약점을 보완하기 위한 실험용 클래스
 *  스캔시 영역을 계산해  서로 연결된 obejct 를 스캔
 *
 *  스캔시 발생하는 점들의 연결성을 죄다 관리하므로
 *  많은 메모리를 사용하지만 복잡한 구조의 스캔이 가능하도록 한다.
 *
 *
 *
 */

#ifndef DCOBJSCAN_H_
#define DCOBJSCAN_H_

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/**
 * @brief 오브젝트 구조의 기본단위
 * @details
 * layer 는 층의 숫자이며
 * s ~ e 까지의 1차원 영역을 의미
 *
 * 예를 들어 종스캔의 경우
 * layer : 10 , s : 1, e : 20
 * 인 rod 는 Point(1,10) ~ Point(20,10) 의 영역을 의미
 *
 */

enum scan_dir {
	dir_v,
	dir_h
};

typedef struct _rod // 스캔 라인의 독립 연결점
{	int layer; 	// horizontal scan case = x , vertical case = y
	int s,e;  	// start , end point
} rod;

typedef vector<rod> bunch;
typedef bunch::iterator Iter_rods;

/**
 * @brief rod의 벡터로 구성된 오브젝트 자료형
 * @details
 * 이미지의 모양을 저장하기 위해 각 층별 층(layer) 시작점(s) ~ 끝점(e) 로 구성된
 *  rod 의 집합으로 표현.
 *
 * @param  vector<rod> rods rod 의 집합
 * @param  int direction 	스캔 방향
 */
class obj {

public:
	int direction; 			// 1 : horizontal 0 : vertical

	bunch rods;

	void merge(obj o);

	void sort();
	int	getVolume(); 		// object 의 부피 (총 pixel 숫자) 를 리턴한다.
	bool isMeet(rod rd1);
	void append(rod rd);

	void draw(Mat img, Scalar color);

	Rect getRect();
private:

};
typedef bunch::iterator  Iter_bunch;
typedef vector<obj>::iterator Iter_obj;

/**
 * @brief 이미지의 오브젝트를 추출하기 위한 클래스
 * @details
 * 횡스캔, 종스캔 양쪽을 다 지원하며
 * vector <obj> objects; 멤버에 저장된다.
 *
 */
class objscan {
private:

	Mat gray;
	bunch mergeTiss( bunch b1, bunch b2);

public:
	int 	min_thresh; 				// min_thresh > 0  인경우 object 구성시 일정 수치 이하의 오브젝트는 필터링함
	Size 	img_size;
	bool 	dmsg;
	vector <obj> objects;

	void draw_all(Mat  gimg, Scalar color);

	void loadImage(Mat img);
	objscan();
	virtual ~objscan();

	void scan(int direction);
	void scanV();
	void scanH();
};



#endif /* DCOBJSCAN_H_ */
