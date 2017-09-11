/*
 * dcTile.cpp
 *
 *  recreated on: 2017. 9. 11.
 *      Author: decoz
 */

#include "tile.h"
namespace srlib{
tile::tile() {
	joints.push_back(Point(0,0));
}

tile::~tile() {
	// TODO Auto-generated destructor stub
}


Point tile::getMaxXY(){

	int mx = 0, my = 0;

	for(int i=0;i<rects.size();i++){
		Rect r = rects[i];
		if( mx < r.x + r.width ) mx = r.x + r.width;
		if( my < r.y + r.height ) my = r.y + r.height;
	}

	return Point(mx,my);
}

Mat  tile::adjustSize(Mat img){

	if(!fixw && !fixh) return img;

	Mat  rimg;
	if(fixw > 0 && fixh > 0){
		resize(img, rimg, Size(fixw, fixh));
		return rimg;
	}

	if(fixh > 0) {
		int w = round( (float)fixh /  img.rows  *  img.cols);
		if(w < 1) w = 1;
		resize(img, rimg, Size(w, fixh));
	}

	if(fixw > 0) {
		int h = round( (float)fixw /  img.cols  * img.rows);
		if(h < 1) h = 1;
		resize(img, rimg, Size(fixw, h));
	}

	return rimg;

}

Mat tile::toBGR(Mat img){

	if(img.channels() == 1) cvtColor(img,img,CV_GRAY2BGR);
	return img;
}

void tile::attach(short opt, Mat img){






}



void tile::fixSize(int w, int h){

	fixw = w;
	fixh = h;

}

Mat tile::mat(){
	Point p = getMaxXY();

	if(p.x == 0 || p.y == 0) return Mat(100,100, CV_8UC3);

	Mat img(p.y, p.x, CV_8UC3);

	for(int i=0;i<rects.size();i++){
		Rect r = rects[i];
		imgs[i].copyTo( img(r) );
	}

	return img;
}
} // end of srlib
