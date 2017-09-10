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

			draw_denshist(src, view, 100);
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

Mat cview::draw_denshist(Mat src, Mat target, int height){
	Mat gray;


	/*
	if(src.channels() > 1) cvtColor(src,gray, CV_BGR2GRAY);
	else gray = src;
	*/

	Mat xhist, yhist;
	reduce(src,xhist,0,CV_REDUCE_AVG);
	reduce(src,yhist,1,CV_REDUCE_AVG);

	int ch = xhist.channels();


	for(int d=0; d<2; d++)
	for(int i=0; i<( d? xhist.cols : yhist.rows ); i++){

		//Point sp = d? Point(i, src.rows) : Point( src.cols, i );

		Vec3b v = d? xhist.at<Vec3b>(0,i) : yhist.at<Vec3b>(i,0);


		int sv = 0;
		for(int c=0; c<ch; c++){
			Scalar color(100,0,0);
			color.val[c] = 150;
			printf("%d ", v[c]);


			//Point ep = d? 	Point(i,  src.rows + v[c] * height / 255) :	Point( src.cols + v[c] * height / 255, i) ;
			Vec3b colorv = Vec3b(color.val[0], color.val[1], color.val[2]);


			if(d)	target.at<Vec3b>( src.rows + v[c] * height / 255, i ) = colorv;
			else 	target.at<Vec3b>( i, src.cols + v[c] * height / 255 ) = colorv;

			sv += v[c];

		}


		if( d )	target.at<Vec3b>( src.rows + sv * height / 3 / 255, i )= Vec3b(255,255,255);
		else target.at<Vec3b>( i, src.cols + sv * height / 3 / 255 )= Vec3b(255,255,255);

	}


	printf("[%d] %d,%d\n", ch, xhist.rows, xhist.cols);

	printf("[%d] %d,%d\n", ch, yhist.rows, yhist.cols);
	fflush(stdout);
	return gray;

}




} // end of srlib
