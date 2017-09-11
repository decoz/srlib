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
 * @brief Ÿ���̹��� ������Ŭ����
 * @details
 *
 * �����̹����� ���̱� ���� Ŭ�����μ� ������ ���� ��带 �����Ѵ�.
 * fixw, fixh : Ÿ�� �̹����� ũ�⸦ ������Ű�� ���
 * maxw, maxh : �̹����� ��ü ����, ũ�⸦ �����ϴ� ���
 *
 * ���� ������ 0���� Ŭ ��� ���� �� ȿ���� ��Ÿ���� �ȴ�
 *
 */

class tile {
private:
	int maxw, maxh;
	int winw, winh; // â�� ���� ���̸� ���� �Ǵ� ���ÿ� fix �ص�  free �� ��� 0 ���� ����
	int fixw, fixh; 		// image �� ���� ���̸� ���� �Ǵ� ���ÿ� fix �ص�

	vector <Rect> rects;
	vector <Mat> imgs;
	vector <Point> joints;


	Point  	getMaxXY();
	Mat  	adjustSize(Mat img);
	Mat 	toBGR(Mat img);

	bool	available(Rect r, Point p);	// r������ p ������ ���� �����Ѱ� Ȯ��
	bool 	intersect(Rect r1, Rect r2); // r1�� r2�� �����ϴ°� Ȯ��


public:
	tile();
	virtual ~tile();

	void fixSize(int w, int h);			// ũ�⸦ fix �� 0�� ��� �ش����
	void attach(short opt, Mat img);		// �̹����� �߰� �ɼ�
											// TILE_H		: ������ �߰�
											// TILE_V 		: �Ʒ��ʿ� �߰�
	
	Mat mat();	// tile image�� ����

};
} // end of srlib
#endif /* DCTILE_H_ */
