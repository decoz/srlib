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

	int 	line_max_width, line_min_length;			// line ����� ���� �Ӱ谪,  line ����� �ּ� ����
	int		adp_thresh;									// adp ����� thresh
	int 	assemble_range;								// ���ս� ���鰣�� �ִ�Ÿ�
	float	assemble_thresh;							// line ����� �ּ� ����

	bool  merge_other_obj;							// �̹��������� ����� ������ �մ´�.

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
