/*
 * filmScanner.h
 *
 *  Created on: 2016. 12. 21.
 *      Author: decoz
 *
 */
#ifndef SECTORSCANNER_H_
#define SECTORSCANNER_H_

#include "objscan.h"
#include "peucker.h"

using namespace cv;
using namespace std;

namespace srlib {

/**
 * @brief film 스캔된 개별 오브젝트
 *
 */
typedef struct {
	obj *hobj, *vobj;
	vector <Path> paths;
} Xobj ;

typedef vector<Xobj*>::iterator Iter_xobj;

/**
 * @brief 결함(Defect)를 표현하기 위한 자료
 * @details
 *
 *  p1, p2 :
 *  단선의 경우 			:  양 단절점을 의미
 *  파임, 돌출의 경우 	:  파임, 돌출 영역의 시작 ~ 끝점을 의미
 *  잔여물, 구멍의 경우	:  해당 4각영역의 좌상점, 우하점을 의미
 *
 */


/**
 * @brief film 이미지를 스캔하는 스캐닝 클래스
 * 	@details
 *
 * 	image 를 상하, 좌우로 두번 스캔하여
 * 	연결된 구역단위로 분리하고 이들중 좁고
 * 	긴 영역으로부터  라인을 추출한다.
 *
 */

enum {
	flag_draw_point  	= 1 << 0,
	flag_draw_edge 	= 1 << 1,
	flag_rand_color 	= 1 << 2
};


class linescan : public objscan {

private:
	void calc_path(Xobj *xobj);
	void draw_path(Mat img, Path ph, Scalar color, uchar  drawflag = 0);
	void assemble_path(Xobj *xobj);

	bool link_point(vector <Path> &paths, Point p, int dir);

	vector <Xobj*> scanX(Mat gimg);


	Path merge_path(Path ph1, Path ph2 , bool hReverse , bool tReverse);
	vector <Path>  assemble_expath(vector <Path> paths);
	vector <Path> 	scanPath(vector <Xobj*> xobjs, bool mode);

public:
	vector <Xobj*> xobjs;
	vector <Path> paths;

	int 	line_max_width, line_min_length;			// line 추출시 넓이 임계값,  line 추출시 최소 길이
	int		adp_thresh;									// adp 적용시 thresh
	int 	assemble_range;								// 병합시 점들간의 최대거리
	float	assemble_thresh;							// line 추출시 최소 길이

	bool  merge_other_obj;							// 이미지상으로 연결된 영역만 잇는다.

	bool debug;
	Mat 	bimg, dbgimg;

	linescan();
	virtual ~linescan();

	void 	scanline(Mat gimg);
	void 	DrawPaths(Mat img);

};

#define  MAX_W 10000
#define  MAX_H 10000

} /* namespace filmInspector */

#endif /* SECTORSCANNER_H_ */
