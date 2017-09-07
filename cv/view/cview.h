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


typedef struct _property{
	bool xyhistogram;
} property;

class cview {
private:

	cview();

public:
	static cview* that;
	static Scalar textcolor;
	static void handle_mouse(int evt, int x, int y, int flags, void *param);


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

	static void show(char *name, Mat img){
		getIt()->_show(name, img);
	}

	void _show(char *name, Mat img);
	void _show_mouse_pos(char *name);
	void _setProperty(char *name, char *pname,const char *pvalue);
	Size _getSize(property *pp);

	virtual ~cview();

};

}

#endif /* OPENCV_LAB_DCCVIEW_DCCVIEW_H_ */
