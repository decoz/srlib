/*
 * dcTileView.cpp
 *
 *  Created on: 2015. 9. 21.
 *      Author: decoz
 *

 *
 */


#include "../tile/dcTileView.h"

dcTileView::dcTileView() {
	// TODO Auto-generated constructor stub


}

dcTileView::~dcTileView() {
	// TODO Auto-generated destructor stub
}


Point dcTileView::getMaxXY(){

	int mx = 0, my = 0;

	for(int i=0;i<rects.size();i++){
		Rect r = rects[i];
		if( mx < r.x + r.width ) mx = r.x + r.width;
		if( my < r.y + r.height ) my = r.y + r.height;
	}

	return Point(mx,my);
}

Mat  dcTileView::adjustSize(Mat img){

	if(!fixw && !fixh) return img;

	Mat  rimg;
	if(fixw > 0 && fixh > 0){
		resize(img, rimg, Size(fixw, fixh));
		return rimg;
	}

	if(fixh > 0) {
		int w = round( (float)fixh /  img.rows  *  img.cols);
		resize(img, rimg, Size(w, fixh));
	}

	if(fixw > 0) {
		int h = round( (float)fixw /  img.cols  * img.rows);
		resize(img, rimg, Size(fixw, h));
	}

	return rimg;

}

Mat dcTileView::toBGR(Mat img){

	if(img.channels() == 1) cvtColor(img,img,CV_GRAY2BGR);
	return img;
}

void dcTileView::attachDown(Mat img){

	Mat aimg	= adjustSize(toBGR(img));

	int max = getMaxXY().y;

	imgs.push_back(aimg);
	Rect r(0, max, aimg.cols, aimg.rows ) ;

	rects.push_back(r);

}

void dcTileView::fixSize(int w, int h){

	fixw = w;
	fixh = h;

}

void dcTileView::show(string wname){

	Point p = getMaxXY();

	Mat img(p.y, p.x, CV_8UC3);

	for(int i=0;i<rects.size();i++){
		Rect r = rects[i];
		imgs[i].copyTo( img(r) );
	}

	imshow(wname, img);
}

