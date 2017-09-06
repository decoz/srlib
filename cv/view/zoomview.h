/**
 * zoomview.h
 *
 *  Created on: 2017. 2. 17.
 *      Author: decoz
 *
 *  @brief zoom�� �����ϴ� viewer
 *
 */

#ifndef ZOOMVIEW_H_
#define ZOOMVIEW_H_

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

namespace srlib {

typedef struct {
	Mat src, view;
	Size wsize;
	float zrate;
	Point pt;
} ZInfo;

/**
 * @brief zoom�� �����ϴ�singleton class viewer
 * @details
 *
 * ��뿹:
 * 	zoomview::show("zview", img, Size(100,100));
 *
 * �̶� Size �� â��  ��, �� max ����� �ǹ��ϸ�
 * �׻� �� ������ �ǹ��Ѵ�. '
 *
 * ������� ���� ���� �̹����� 200,100 �̸�
 * 100, 50 �� â ũ�⸦ ���ϸ�
 * ���ٷ� 100,200�̸� 50, 100�� â�� �����ȴ�.
 *
 */
class zoomview {
private:
	zoomview();

public:
	static Scalar 			textcolor;
	static zoomview* 	it;
	static 	zoomview* 	getIt(){	if(it == NULL) it = new zoomview(); return it; }
	static int sx, sy;

	map<string, ZInfo*> zinfo;

	static void handle_mouse(int evt, int x, int y, int flags, void *param);
	static void show(char *name, Mat img, Size sz){
		getIt()->_show(name, img, sz);
	}
	static void release(char *name){
		getIt()->_release(name);
	}

	void 	 _show(char *name, Mat img, Size sz);
	void 	_release(char *name);

	virtual ~zoomview();
};






} /* namespace sr_lib */

#endif /* ZOOMVIEW_H_ */
