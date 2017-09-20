
/*
 * dsCView.h
 *
 *  Created on: 2016. 4. 4.
 *      Author: decoz
 *
 *  opencv ���� ������� ���ϰ� �ϱ� ����
 *  imshow �� ����� ��ȭ�ϱ� ���� Ŭ����
 *  �켱������ ���콺 ��ǥ ��±�ɺ���..
 *
 *  ������
 *
 *  ::show("name",img1);
 *
 *  �������� ����� �� �ֱ⸦ ..
 *
 */

#ifndef OPENCV_LAB_DCCVIEW_DCCVIEW_H_
#define OPENCV_LAB_DCCVIEW_DCCVIEW_H_

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace srlib{


typedef void (*Handler)(Mat img, void* userdata); // event handler



typedef struct _property{
	Rect dragrect;


	bool xyhistogram;				// xy histogram  �� ���
	bool xyposition;

	Handler evt_dragrect; 	// rectangle drag event
} property;

class cview {
private:

	cview();

public:
	static cview* that;
	static Scalar textcolor;
	static void handle_mouse(int evt, int x, int y, int flags, void *param);
	static void handle_dragrect(int evt, int x, int y, int flags, void *param);

	map<string, Mat> imgs;
	map<string, property*> pmap;

	static 	cview* getIt(){
		if(that == NULL)
			that = new cview();
		return that;
	}

	static void turnon(char *name, char *pname){
		getIt()->_setProperty(name, pname, "true");
	}

	static void set(char *name, char *pname, char *pvalue){
		getIt()->_setProperty(name, pname, pvalue);
	}

	static void handle(char *name, char *ename, Handler handler ){
		getIt()->_setEvent(name, ename, handler);
	}


	static void show(char *name, Mat img){
		getIt()->_show(name, img);
	}

	Mat draw_denshist(Mat src, Mat target,  int height);
	void _show(char *name, Mat img);
	void _show_mouse_pos(char *name);
	void _setProperty(char *name, char *pname,const char *pvalue);
	void _setEvent(char *name, char *ename, Handler handler );

	Size _getSize(property *pp);

	virtual ~cview();

};

}

#endif /* OPENCV_LAB_DCCVIEW_DCCVIEW_H_ */

