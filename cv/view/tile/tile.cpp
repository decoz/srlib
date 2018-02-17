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

	maxx = maxy = 0;
	winw = winh = 0;
	margin = 0;
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

bool tile::available(Rect r, Point p){

	Rect tr(p.x, p.y, r.width, r.height);


	if( 	( winw > 0 &&  p.x + r.width > winw ) ||
		( winh > 0 &&  p.y + r.height > winh ) )
		return false;


	for(vector<Rect>::iterator i=rects.begin(); i!=rects.end(); i++){
		//if(!interRect(tr, *i).width) return false;

		r = interRect(tr, *i);
		//printf("/r.w:%d h:%d ",r.width, r.height);
		if(r.width > 0 && r.height > 0)  return false;

	}
	return true;
}



void tile::attach(short opt, Mat img){

	typedef vector<Point>::iterator Iter_point;
	Rect r(0,0,img.cols + margin,img.rows + margin);

	Iter_point i_minp;
	double minval = 0;

	//printf("jp list:");
	for(vector<Point>::iterator i=joints.begin(); i!=joints.end(); i++){
		Point p = *i;
		//printf("%d,%d.", p.x, p.y);
		if(!available(r,p)) continue;

		int tx = (p.x + r.width < maxx) ? maxx : p.x + r.width,
			ty = (p.y + r.height < maxy) ? maxy : p.y + r.height;

		//printf("[%d,%d] ",tx,ty);
		if(!minval || tx * ty < minval)
			i_minp = i, minval = tx * ty;
	}



	//printf("\n");

	if(minval > 0 ) {
		Point jp = *i_minp;
		joints.erase(i_minp);
		joints.push_back(Point(jp.x + r.width, jp.y));
		joints.push_back(Point(jp.x , jp.y + r.height));

		maxx = (jp.x + r.width < maxx) ? maxx : jp.x + r.width,
		maxy = (jp.y + r.height < maxy) ? maxy : jp.y + r.height;

		rects.push_back( Rect(jp.x, jp.y, r.width, r.height));
		imgs.push_back(img);

	}
	//printf("new max : %d,%d\n", maxx, maxy);


}



void tile::fixSize(int w, int h){

	fixw = w;
	fixh = h;

}

Mat tile::mat(){

	//printf("t.size:%d\n", rects.size() );
	Point p = getMaxXY();


	if(p.x == 0 || p.y == 0 || !imgs.size()) return Mat(100,100, CV_8UC1);

	Mat img(p.y, p.x, imgs[0].type());
	img.setTo(0);

	for(int i=0;i<rects.size();i++){
		Rect r = rects[i];
		//printf("%d: -> %d,%d,%d,%d\n", i, r.x, r.y, r.width, r.height);
		fflush(stdout);
		int c1 = imgs[i].type(), c2 = img(r).type();

		Rect  nr(r.x, r.y, r.width - margin ,  r.height - margin);
		imgs[i].copyTo( img(nr) );
		//imshow("imgs",imgs[i]);	if ( waitKey(0) == 'q') break;
	}

	return img;
}


Rect tile::interRect(Rect r1, Rect r2){
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

} // end of srlib
