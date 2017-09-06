/**
 * zoomview.h
 *
 *  Created on: 2017. 2. 17.
 *      Author: decoz
 *
 *  @brief zoom을 지원하는 viewer
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
 * @brief zoom을 지원하는singleton class viewer
 * @details
 *
 * 사용예:
 * 	zoomview::show("zview", img, Size(100,100));
 *
 * 이때 Size 는 창의  좌, 우 max 사이즈를 의미하며
 * 항상 원 비율을 의미한다. '
 *
 * 예를들어 만일 원래 이미지가 200,100 이면
 * 100, 50 의 창 크기를 지니며
 * 꺼꾸로 100,200이면 50, 100의 창이 생성된다.
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
