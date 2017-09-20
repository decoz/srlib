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



void cview::handle_dragrect(int evt, int x, int y, int flags, void *param){

	char *name = (char*) param;

	cview *it = cview::getIt();
	Mat img = it->imgs[name];
	property *pp = it->pmap[name];

	switch(evt){
	case CV_EVENT_MOUSEMOVE:
		break;
	case CV_EVENT_LBUTTONDOWN:
		pp->dragrect = Rect(x,y,0,0);
		printf("lbdown\n"); break;
	case CV_EVENT_LBUTTONUP:
		Rect dr = pp->dragrect;;
		pp->dragrect = Rect(
			dr.x < x ? dr.x : x,
			dr.y < y ? dr.y : y,
			abs( dr.x - x ),
			abs( dr.y - y )
		);

		dr = pp->dragrect;
		printf("rect [%d,%d,%d,%d]\n", dr.x, dr.y, dr.width, dr.height);
		pp->evt_dragrect(img, pp);
		break;
	}



	fflush(stdout);
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

	namedWindow(name);

	if( pmap.find(name) != pmap.end() ) {
		property p = *pmap[name];
		if(p.xyhistogram) {
			Size vsize = Size(src.cols + 100, src.rows + 100);
			if( view.size() != vsize )
				imgs[name] = view = Mat( vsize,  src.type());
			view.setTo(0);
			Mat vsrc = view(Rect(0,0,src.cols, src.rows));
			src.copyTo(vsrc);
			draw_denshist(src, view, 100);
		}

		if(p.xyposition) {
			setMouseCallback(name, cview::handle_mouse, (void*)name);
		}

		if(p.evt_dragrect != NULL){
			setMouseCallback(name, cview::handle_dragrect, (void*)name);
		}

	}

	imshow(name, view);

}

void init_property(property *pp){
	pp->xyhistogram = false;
	pp->xyposition = false;

	pp->evt_dragrect = NULL;

}

void cview::_setEvent(char *name, char *ename, Handler handler ){
	property *pp;
	if( pmap.find(name) == pmap.end() ) {
		pp = new property;
		init_property(pp);
		pmap[name] = pp;
	} pp = pmap[name];

	if( !strcmp(ename,"evt_dragrect") ){
		pp->evt_dragrect = handler;
	}
}



void cview::_setProperty(char *name, char *pname,const char *pvalue){
	printf("get %s / %s  : %s\n", name, pname, pvalue);
	property *pp;

	if( pmap.find(name) == pmap.end() ) {
		pp = new property;
		init_property(pp);
		pmap[name] = pp;
	} pp = pmap[name];

	if( !strcmp(pname,"xyposition") )
		if( !strcmp(pvalue,"true") ) pp->xyposition = true;
		else  pp->xyposition = false;

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
		Vec3b v;
		//
		if( ch > 1 ) v = d? xhist.at<Vec3b>(0,i) : yhist.at<Vec3b>(i,0);
		else v = Vec3b( d? xhist.at<uchar>(0,i) : yhist.at<uchar>(i,0), 0, 0  );

		int sv = 0;
		if( ch > 1 ){
			for(int c=0; c<ch; c++){
				Scalar color(100,0,0);
				color.val[c] = 150;
				Vec3b colorv = Vec3b(color.val[0], color.val[1], color.val[2]);

				if(d)	target.at<Vec3b>( src.rows + v[c] * height / 255, i ) = colorv;
				else 	target.at<Vec3b>( i, src.cols + v[c] * height / 255 ) = colorv;

				sv += v[c];

			}
			if( d )	target.at<Vec3b>( src.rows + sv * height / 3 / 255, i )= Vec3b(255,255,255);
			else 	target.at<Vec3b>( i, src.cols + sv * height / 3 / 255 )= Vec3b(255,255,255);
		} else {
			if( d )	target.at<uchar>( src.rows + v[0] * height / 255, i )= 255;
			else 	target.at<uchar>( i, src.cols + v[0] * height / 255 )= 255;
		}

	}

	fflush(stdout);
	return gray;

}




} // end of srlib

