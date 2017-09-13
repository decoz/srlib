/*
 * objscan.cpp
 *
 *  Created on: 2015. 10. 7.
 *      Author: decoz
 */

#include "objscan.h"

#include <stdio.h>

objscan::objscan() {
	dmsg = false;
	min_thresh = 1;
}

objscan::~objscan() {
	// TODO Auto-generated destructor stub
}

void objscan::loadImage(Mat img) {

	objects.clear();
	gray = img;
	//gray = asBgBlack(asGray(img));

	/*
	if(img.channels() > 1) cvtColor(img,gray,CV_BGR2GRAY);
	else gray = img;
	*/

	img_size = img.size();

}

/** @brief obj 전체를 img 에 그린다.*/
void objscan::draw_all(Mat  gimg, Scalar color){
/*
 *  obj 전체를 img 에 그린다.
 *  min_thresh 와 같이 사용시 noise_filtering 이 가능
 */

	for(Iter_obj i = objects.begin(); i != objects.end();  i++ )
		i->draw(gimg, color );
}

/**
 * @brief binary image 를 스캔하여 각각의 구분된  오브젝트를 추출
 */
void objscan::scan(int d){
	vector <obj> current, next;
	bool dmsg = false;
	for(int y=0;y<(d? gray.cols:gray.rows);y++){
		for(int x=0, s=-1; x<=(d? gray.rows:gray.cols);x++)
			if(  x < (d? gray.rows:gray.cols) && (d? gray.at<uchar>(x,y) : gray.at<uchar>(y,x) ) > 127 ){
				if( s < 0 ) s = x;
			}	else if( s >= 0){  					// create rod and check if it meets current objects
				rod nrd = {y, s, x-1};
				Iter_obj meet = current.end();
				for(Iter_obj i = current.begin(); i!=current.end();  i++)
					if( i->isMeet(nrd) )
						if( meet == current.end() ) {
							i->append(nrd);  		// meet first
							meet = i;
						} else {
							Iter_obj n = i-1;
							meet->merge( *i );		// meet more than one
							current.erase(i);
							i = n;
						}
				if(meet == current.end()) {// meet none -> create obj
					obj nobj;
					nobj.direction = d;
					nobj.append(nrd);
					current.push_back(nobj);
				}
				s = -1;
			}
		/* select for next */
		next.clear();
		for(Iter_obj i=current.begin(); i!=current.end(); i++){
			bunch rods = i->rods;
			rod last =*(rods.end() - 1);

			if(last.layer == y )	next.push_back(*i);
			else if(i->getVolume()  >= min_thresh   )
				objects.push_back(*i);
		}
		current = next;
	}

	for(Iter_obj i=current.begin(); i!=current.end(); i++)
		if(i->getVolume()  >= min_thresh )	objects.push_back(*i);

	if(dmsg) printf("scan result: %d objects \n", objects.size());
}

void objscan::scanV(){
	/* old method */
	scan(1);
}

void objscan::scanH(){
	/* old method */
	scan(0);
}


bool comprod(rod r1,  rod r2){
	if(r1.layer < r2.layer) return true;
	else return false;
}

void obj::draw(Mat img, Scalar color){

	for(int i=0;i<rods.size();i++){
		rod tr = rods[i];
		for(int j=tr.s; j<=tr.e; j++)
			if(img.channels() == 3 ) {
				if(direction)  img.at<Vec3b>(j, tr.layer) = Vec3b( color.val[0], color.val[1], color.val[2] );
				else  img.at<Vec3b>( tr.layer, j) = Vec3b( color.val[0], color.val[1], color.val[2] );
			} else if(img.channels() == 1 ) {
				if(direction)  img.at<uchar>(j, tr.layer) = color.val[0];
				else  img.at<uchar>( tr.layer, j) = color.val[0];
			}
	}


}

int  obj::getVolume(){

	int sum = 0;
	for(int i=0;i<rods.size();i++)
		sum += rods[i].e - rods[i].s + 1;

	return sum;
}


void obj::sort(){
	std::sort( rods.begin(), rods.end(), comprod);
}

Rect  obj::getRect(){

	int l = rods.size();
	int sl = rods[0].layer, el = rods[l-1].layer;

	int min = rods[0].s , max = rods[0].e;

	for(int i=1;i<l;i++){

		if(sl > rods[i].layer ) sl = rods[i].layer;
		if(el < rods[i].layer ) el = rods[i].layer;

		if(min > rods[i].s) min = rods[i].s;
		if(max < rods[i].e) max = rods[i].e;
	}

	Rect r;

	if(direction) {
		r.x = sl;
		r.y = min;
		r.width = el - sl + 1;
		r.height = max - min +  1;
	}
	else {
		r.x = min;
		r.y = sl;
		r.width = max - min + 1;
		r.height = el - sl + 1;
	}


	return r;
}


void obj::merge( obj o){



	for(int i=0;i<o.rods.size();i++)
		rods.push_back(o.rods[i]) ;

	sort();
}

void obj::append(rod rd){
	rods.push_back(rd);
}


bool obj::isMeet( rod rd1 ){

	Iter_rods er = rods.end() -1;
	while( er->layer >= rd1.layer -1 ){
		if( er->e >= rd1.s  && er->s <= rd1.e ) return true;

		if( er == rods.begin() ) break;
		else er--;
	}

	return false;
}



