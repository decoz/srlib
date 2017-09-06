/*
 * dsCView.h
 *
 *  Created on: 2016. 4. 4.
 *      Author: decoz
 *
 *  opencv 에서 디버깅을 편하게 하기 위해
 *  imshow 의 기능을 강화하기 위한 클래스
 *  우선적으로 마우스 좌표 출력기능부터..
 *
 *  사용법은
 *
 *  ::show("name",img1);
 *
 *  이정도로 사용할 수 있기를 ..
 *
 */

#ifndef OPENCV_LAB_DCCVIEW_DCCVIEW_H_
#define OPENCV_LAB_DCCVIEW_DCCVIEW_H_

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class cview {
private:

	cview();

public:
	static cview* that;
	static Scalar textcolor;
	static void handle_mouse(int evt, int x, int y, int flags, void *param);

	map<string, Mat> imgs;

	static 	cview* getIt(){
		if(that == NULL)
			that = new cview();
		return that;
	}

	static void show(char *name, Mat img){
		getIt()->_show(name, img);
	}

	void _show(char *name, Mat img);
	void _show_mouse_pos(char *name);

	virtual ~cview();

};



#endif /* OPENCV_LAB_DCCVIEW_DCCVIEW_H_ */
