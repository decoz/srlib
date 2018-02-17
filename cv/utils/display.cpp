#include "util.h"

namespace srlib {
void putMultiLine(Mat img, const string& text, Point org,	int fontFace, double fontScale, Scalar color, int l_height = 15){

	String linetext = "";

	for(int i=0;i<text.length();i++){
		if(text[i] != 10) linetext += text[i];
		else {
			putText( img, linetext , org, fontFace , fontScale, color );
			linetext = "";
			org.y += l_height;
		}
	}

	if(linetext != "") putText( img, linetext , org, fontFace , fontScale, color );
}


Scalar randcolor(int min, int max){
/*
 *  랜덤한 컬러를 리턴한다.
 *	밝기의 범위를 줄 수 있다.
 */

	int range = abs( max - min ) * 3;
	int sum = min*3 + rand()%range;

	int cv[3];
	Scalar color(0,0,0);
	for(int i=0;i<3;i++){
		int rmax = sum>255?255:sum;
		cv[i] = rand() % rmax;
		sum -= cv[i];
	}

	return Scalar(cv[0], cv[1], cv[2]);

}

void drawMark(Mat img, Point p1,int size,Scalar color){

	int  x1,x2,x3,y1,y2,y3;

	x1 = p1.x - size;
	if(x1 < 0) x1 = 0;

	x2 = p1.x;

	x3 = p1.x + size;
	if(x3 >= img.cols) x3 = img.cols - 1;

	y1 = p1.y - size;
	if(y1 < 0) y1 = 0;

	y2 = p1.y;

	y3 = p1.y + size;
	if(y3 >= img.rows) y3 = img.cols - 1;


	line( img, Point(x1,y2), Point(x3,y2), color);
	line( img, Point(x2,y1), Point(x2,y3), color);

}
} // namespace srlib

