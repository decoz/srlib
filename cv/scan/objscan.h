/*
 * objscan.h
 *
 *  Created on: 2015. 10. 7.
 *      Author: decoz
 *
 *  ocr div �� ������ �����ϱ� ���� ����� Ŭ����
 *  ��ĵ�� ������ �����  ���� ����� obejct �� ��ĵ
 *
 *  ��ĵ�� �߻��ϴ� ������ ���Ἲ�� �˴� �����ϹǷ�
 *  ���� �޸𸮸� ��������� ������ ������ ��ĵ�� �����ϵ��� �Ѵ�.
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
 * @brief ������Ʈ ������ �⺻����
 * @details
 * layer �� ���� �����̸�
 * s ~ e ������ 1���� ������ �ǹ�
 *
 * ���� ��� ����ĵ�� ���
 * layer : 10 , s : 1, e : 20
 * �� rod �� Point(1,10) ~ Point(20,10) �� ������ �ǹ�
 *
 */

enum scan_dir {
	dir_v,
	dir_h
};

typedef struct _rod // ��ĵ ������ ���� ������
{	int layer; 	// horizontal scan case = x , vertical case = y
	int s,e;  	// start , end point
} rod;

typedef vector<rod> bunch;
typedef bunch::iterator Iter_rods;

/**
 * @brief rod�� ���ͷ� ������ ������Ʈ �ڷ���
 * @details
 * �̹����� ����� �����ϱ� ���� �� ���� ��(layer) ������(s) ~ ����(e) �� ������
 *  rod �� �������� ǥ��.
 *
 * @param  vector<rod> rods rod �� ����
 * @param  int direction 	��ĵ ����
 */
class obj {

public:
	int direction; 			// 1 : horizontal 0 : vertical

	bunch rods;

	void merge(obj o);

	void sort();
	int	getVolume(); 		// object �� ���� (�� pixel ����) �� �����Ѵ�.
	bool isMeet(rod rd1);
	void append(rod rd);

	void draw(Mat img, Scalar color);

	Rect getRect();
private:

};
typedef bunch::iterator  Iter_bunch;
typedef vector<obj>::iterator Iter_obj;

/**
 * @brief �̹����� ������Ʈ�� �����ϱ� ���� Ŭ����
 * @details
 * Ⱦ��ĵ, ����ĵ ������ �� �����ϸ�
 * vector <obj> objects; ����� ����ȴ�.
 *
 */
class objscan {
private:

	Mat gray;
	bunch mergeTiss( bunch b1, bunch b2);

public:
	int 	min_thresh; 				// min_thresh > 0  �ΰ�� object ������ ���� ��ġ ������ ������Ʈ�� ���͸���
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
