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
 * @brief film ��ĵ�� ���� ������Ʈ
 *
 */
typedef struct {
	obj *hobj, *vobj;
	vector <Path> paths;
} Xobj ;

typedef vector<Xobj*>::iterator Iter_xobj;

/**
 * @brief ����(Defect)�� ǥ���ϱ� ���� �ڷ�
 * @details
 *
 *  p1, p2 :
 *  �ܼ��� ��� 			:  �� �������� �ǹ�
 *  ����, ������ ��� 	:  ����, ���� ������ ���� ~ ������ �ǹ�
 *  �ܿ���, ������ ���	:  �ش� 4�������� �»���, �������� �ǹ�
 *
 */


/**
 * @brief film �̹����� ��ĵ�ϴ� ��ĳ�� Ŭ����
 * 	@details
 *
 * 	image �� ����, �¿�� �ι� ��ĵ�Ͽ�
 * 	����� ���������� �и��ϰ� �̵��� ����
 * 	�� �������κ���  ������ �����Ѵ�.
 *
 */
class linescan : public objscan {

	void calc_path(Xobj *xobj);
	void draw_path(Mat img, Path ph, Scalar color);
	void assemble_path(Xobj *xobj);

	vector <Xobj*> scanX(Mat gimg);


	Path merge_path(Path ph1, Path ph2 , bool hReverse , bool tReverse);

	vector <Path> 	scanPath(vector <Xobj*> xobjs, bool mode);
	void scan_rvalue(Path ph);
	void trace_path(Path ph);


public:
	vector <Xobj*> xobjs;
	vector <Path> paths;

	int 	line_max_width, line_min_length;			// line ����� ���� �Ӱ谪,  line ����� �ּ� ����
	int 	assemble_range;								// ���ս� ���鰣�� �ִ�Ÿ�
	float	assemble_thresh;							// line ����� �ּ� ����

	bool debug;
	Mat 	bimg;

	linescan();
	virtual ~linescan();

	void 	scanline(Mat gimg);
	void 	DrawPaths(Mat img);

};

#define  MAX_W 10000
#define  MAX_H 10000

} /* namespace filmInspector */

#endif /* SECTORSCANNER_H_ */
