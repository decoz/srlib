#include <stdio.h>
#include "util.h"

namespace srlib {


Rect imgrect(Mat img ){ return Rect(0, 0, img.cols, img.rows); }

bool inRectP(Rect r, Point p){

	if( p.x >= r.x && p.x <= r.x + r.width  &&
			p.y >= r.y &&  p.y <= r.y + r.height ) return true;
	else return false;

}

bool inRectR(Rect br, Rect sr){

	if( inRectP(br, Point(sr.x, sr.y) ) &&
		inRectP(br,  Point( sr.x + sr.width, sr.y + sr.height) ))
		return true;
	else return  false;


}


Rect 	interRect(Rect r1, Rect r2){
/*
 * 두 영역중에 교차영역을 계산
 * 교차  영역이 없으면 R(0,0,0,0) 을 리턴
 */
	Rect inter_r;
	Rect lft_r, rgt_r, up_r, dn_r; //  좌/우  상/하

	if(r2.x <= r1.x + r1.width && r1.x <= r2.x + r2.width  ){

		int a = r1.x > r2.x? r1.x : r2.x;
		int b = r1.x + r1.width < r2.x + r2.width ? r1.x + r1.width : r2.x + r2.width;
		inter_r.x = a;
		inter_r.width = b - a;

	}else return  Rect(0,0,0,0);

	if(r2.y <= r1.y + r1.height && r1.y <= r2.y + r2.height  ){

		int a = r1.y > r2.y? r1.y : r2.y;
		int b = r1.y + r1.height < r2.y + r2.height ? r1.y + r1.height : r2.y + r2.height;
		inter_r.y = a;
		inter_r.height = b - a;

	}else return  Rect(0,0,0,0);

	return inter_r;

};


Rect rectsum(Rect r1,Rect r2){

	//if(r1.x <= 0 && r1.width <= 0 ) return r2;
	if(r1.x < 0) return r2;

	Rect r;
	if(r1.x < r2.x ) r.x = r1.x;
	else r.x = r2.x;

	if(r1.y < r2.y ) r.y = r1.y;
	else r.y = r2.y;

	Point p1(r1.x+r1.width,	r1.y+r1.height);
	Point p2(r2.x+r2.width,	r2.y+r2.height);
	Point p;

	if(p1.x > p2.x) p.x = p1.x;
	else p.x = p2.x;

	if(p1.y > p2.y ) p.y = p1.y;
	else p.y = p2.y;

	r.width = p.x - r.x;
	r.height = p.y - r.y;

	return r;
}


vector <Rect> sortRects( vector <Rect> rects, int type, int inc) {
/*
 * rect 들을 정렬
 *
 * type
 * SORT_BY_SX	: start x 로 정렬		(default)
 * SORT_BY_SY	: start y 로 정렬
 * SORT_BY_MX	: x 중점으로 정렬
 * SORT_BY_MY : y 중점으로 정렬
 *
 * inc
 * 1	: incremental  (0,1,2...)			(default)
 * -1	: decrement	(9,8,7...)
 */
	int s = (int) rects.size();
	for(int i=0;i<s-1;i++)
		for(int j=0; j<s-1-j; j++){
			int v1, v2;
			switch(type){
			case SORT_BY_SX :
				v1 = rects[j].x, v2 = rects[j+1].x; break;
			case SORT_BY_SY :
				v1 = rects[j].y, v2 = rects[j+1].y; break;
			case SORT_BY_MX :
				v1 = rects[j].x + rects[j].width;
				v2 = rects[j+1].x + rects[j+1].width;
				break;
			case SORT_BY_MY :
				v1 = rects[j].y + rects[j].height;
				v2 = rects[j+1].y + rects[j+1].height;
				break;
			default:
				i = s+1; 	// break out
			}

			if( ( v1 - v2 ) * inc  > 0) {
				Rect t = rects[j];
				rects[j] = rects[j+1];
				rects[j+1] = t;
			}
		}

	return rects;
}


Rect rectsum(vector <Rect> rects){
	if( rects.size() > 0 ) {
		Rect r = rects[0];
		for(int i=0;i<rects.size();i++)
			r = rectsum(r, rects[i]);
		return r;
	}
	else return Rect(0,0,0,0);
}



Rect 	fitRect(Rect r, Rect a_r){

	if( r.x < a_r.x ) r.x = a_r.x;
	if( r.y < a_r.y ) r.y = a_r.y;

	if( r.x > a_r.x + a_r.width ) r.x = a_r.x;
	if( r.y > a_r.y + a_r.height ) r.y = a_r.y;


	if( r.x + r.width > a_r.x + a_r.width ) r.width =  a_r.x + a_r.width - r.x;
	if( r.y + r.height > a_r.y + a_r.height ) r.height = a_r.y + a_r.height - r.y;

	return r;
}


char* tostr(Rect r){
	char *buff = new char[20];
	sprintf(buff, "%d,%d,%d,%d",r.x,r.y,r.width,r.height);
	return buff;
}

} // namepsace srlib

