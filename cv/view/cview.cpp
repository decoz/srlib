/*
 * dcCView.cpp
 *
 *  Created on: 2016. 4. 4.
 *      Author: decoz
 */

#include "cview.h"
#include <stdio.h>

namespace srlib {

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


Size cview::_getSize(property *pp){


}


void cview::_show(char *name, Mat src){
/*
 * imshow 에 다음의 기능을 추가한다.
 *
 * - img 헤더를 저장
 * - 좌표 업데이트 펑션 추가
 */


	Mat view;
	if( imgs.find(name) ==  imgs.end() ){
		imgs[name] = view = src;
	} else view = imgs[name];

	if( pmap.find(name) != pmap.end() ) {
		property p = *pmap[name];
		if(p.xyhistogram) {
			Size vsize = Size(src.cols + 100, src.rows + 100);
			if( view.size() != vsize )
				imgs[name] = view = Mat( vsize,  src.type());

			Mat vsrc = view(Rect(0,0,src.cols, src.rows));
			src.copyTo(vsrc);
		}
	}

	imshow(name, view);
	setMouseCallback(name, cview::handle_mouse, (void*)name);
}

void init_property(property *pp){

	pp->xyhistogram = false;

}

void cview::_setProperty(char *name, char *pname,const char *pvalue){
	printf("get %s / %s  : %s\n", name, pname, pvalue);
	property *pp;

	if( pmap.find(name) == pmap.end() ) {
		pp = new property;
		init_property(pp);
		pmap[name] = pp;
	} pp = pmap[name];

	if( !strcmp(pname,"xyhistogram") ){
		if( !strcmp(pvalue, "true") ){
			pp->xyhistogram = true;
		}
		if( !strcmp(pvalue, "false") ){
			pp->xyhistogram = false;
		}
	}



}

} // end of srlib
