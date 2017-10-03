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


/**
 * @brief rect 윤곽 라인의  픽셀정보를 저장
 * @details
 * 저장방식은 다음과 같이나선구조로 순서대로 입력된다.
 *  *
 *  AAAB
 *  D   B
 *  D   B
 *  DCCC
 *
 */

Mat save(Mat img, Rect r){

	/*
	int  sz = (  r.width + r.height - 1 ) * 2;
	Mat bk(0,0,img.type());

	bk.push_back( img( Rect(r.x, r.y, r.width-1, 1) ));
	bk.push_back( img( Rect(r.x + r.width, r.y, 1, r.height-1 ) ));
	bk.push_back( img( Rect(r.x+1, r.y + r.height, r.width-1, 1) ));
	bk.push_back( img( Rect(r.x, r.y, 1, r.height-1 ) ));
	*/
	return img(r).clone();

}

void resotre(Mat img, Rect r, Mat arr ){

	arr.copyTo(img(r));

}

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

inline Rect pos2rect(Point p1, Point p2){
	return  Rect(
			p1.x < p2.x ? p1.x :p2.x,
			p1.y < p2.y ? p1.y :p2.y,
			abs( p1.x - p2.x ),
			abs( p1.y - p2.y )
		);
}

void cview::handle_dragrect(int evt, int x, int y, int flags, void *param){

	char *name = (char*) param;

	cview *it = cview::getIt();
	Mat img = it->imgs[name];
	property *pp = it->pmap[name];


	Rect dr = pp->dragrect;
	//Point dp(dr.x, dr.y) , cp(x,y);

	switch(evt){
	case CV_EVENT_MOUSEMOVE:
		if(pp->dragging) {
			if(!pp->dragsave.empty()) resotre(img, dr, pp->dragsave);
			dr = 	pos2rect(pp->dp, Point(x,y));
			pp->dragsave =  save(img, dr );
			rectangle(img, dr, Scalar(255,255,255) );
			pp->dragrect = dr;
			imshow(name, img);
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		if( !pp->dragend ) break; // 이벤트 처리중 다른 drag 진입 금지
		pp->dp = Point(x,y);
		pp->dragrect = Rect(x,y,0,0);
		pp->dragging = true;
		printf("lbdown\n"); break;

	case CV_EVENT_LBUTTONUP:
		if( !pp->dragend ) break; // 이벤트 처리중 다른 drag 진입 금지

		pp->dragrect = pos2rect(pp->dp, Point(x,y));
		dr = pp->dragrect;
		printf("rect [%d,%d,%d,%d]\n", dr.x, dr.y, dr.width, dr.height);

		if(!pp->dragsave.empty()) resotre(img, dr, pp->dragsave);
		imshow(name, img);
		pp->dragsave.release();
		pp->dragging = false;

		pp->dragend = false;		// 임계영역
		if( dr.width * dr.height > 25 ) pp->evt_dragrect(img, pp);
		pp->dragend = true;
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
	/*
	if( imgs.find(name) ==  imgs.end() ){
		imgs[name] = view = src;
	} else view = imgs[name];
	*/
	imgs[name] = view = src;
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
	pp->dragging = false;
	pp->dragend = true;

	pp->userptr = NULL;
}

void cview::_setEvent(char *name, char *ename, Handler handler , void *ptr){
	property *pp;
	if( pmap.find(name) == pmap.end() ) {
		pp = new property;
		init_property(pp);
		pmap[name] = pp;
	} pp = pmap[name];

	pp->userptr = ptr;

	if( !strcmp(ename,"evt_dragrect") ){
		pp->userptr = ptr;
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

