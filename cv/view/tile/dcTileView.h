/*
 * dcTileView.h
 *
 *  Created on: 2015. 9. 21.
 *      Author: decoz
 *
 *  ���� �̹����� �ϳ��� â���� �ٿ��� �����ֱ� ���� ������ Ŭ����
 *
 *	attach  		: �ڵ�(?) .. ���߿� ��������
 *  attachDown 	: �Ʒ��� ���� ����
 *	attachRight   : ���� ��ܿ� ����
 *
 */

#ifndef DCTILEVIEW_H_
#define DCTILEVIEW_H_

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


class dcTileView {
private:
	int maxw, maxh; 	// â�� ���� ���̸� ���� �Ǵ� ���ÿ� fix �ص�  free �� ��� 0 ���� ����
	int fixw, fixh; 		// image �� ���� ���̸� ���� �Ǵ� ���ÿ� fix �ص�

	vector <Rect> rects;
	vector <Mat> imgs;


	Point  getMaxXY();
	Mat  	adjustSize(Mat img);
	Mat 	toBGR(Mat img);


public:
	dcTileView();
	virtual ~dcTileView();

	void fixSize(int w, int h);		// ũ�⸦ fix �� 0�� ��� �ش����
	void attachDown(Mat img);	// �Ʒ��ʿ� ����
	void attachRight(Mat img);	// ������ ����
	void attach(Mat img);	// �ƹ��볪.. �ڵ�..( ���߿� õõ�� ����)

	void show(string wname);


};

#endif /* DCTILEVIEW_H_ */
