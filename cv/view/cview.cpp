/*
 * dcCView.cpp
 *
 *  Created on: 2016. 4. 4.
 *      Author: decoz
 */

#include "cview.h"

#include <stdio.h>

cview *cview::that = NULL;
Scalar cview::textcolor = Scalar(255,0,255);

void cview::handle_mouse(int evt, int x, int y, int flags, void *param){

	char *name = (char*) param;

	cview *it = cview::getIt();
	Mat img = it->imgs[name];

	char text[100];

	Point 	text_pos(10,30);
	Rect 	text_rect( text_pos.x, text_pos.y - 30, 100, 30);
	Mat backup = img(text_rect).clone();

	switch(evt){
	case CV_EVENT_MOUSEMOVE:
		sprintf(text,"%d,%d",x,y);
		putText(img,text,Point(10,20),FONT_HERSHEY_SCRIPT_COMPLEX,0.6,cview::textcolor);

		imshow(name,img);
		backup.copyTo(img(text_rect));


	}
}

cview::cview() {
	// TODO Auto-generated constructor stub

}

cview::~cview() {
	// TODO Auto-generated destructor stub
}

void cview::_show(char *name, Mat img){
/*
 * imshow 에 다음의 기능을 추가한다.
 *
 * - img 헤더를 저장
 * - 좌표 업데이트 펑션 추가
 */

	imshow(name, img);
	imgs[name] = img;

	setMouseCallback(name, cview::handle_mouse, (void*)name);


}
