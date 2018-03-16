/*
 * peucker.cpp
 *
 *  Created on: 2017. 1. 13.
 *      Author: decoz
 */

#include "peucker.h"

#include "scanlib.h"

namespace srlib {


double get_measure(Point sp, Point ep, Point p, int mode){

	double l = range( sp, ep);

	if(l <  1) l = 1;

	switch(mode){
	case MODE_DP : return ( l > 1 ? abs( peppInLength( sp, ep, p )) / l : range( Point( (ep.x+sp.x)/2,(ep.y+sp.y)/2),  p) / l ) * 100  ;
	case MODE_ADP :	return range(sp,p) * range(p,ep)  / l;
	default : return 0;
	}

}

Iter_point  _pecker(Path *pecker_path, Iter_point pos,  Iter_point  is, Iter_point ie, double  mind, int mode) {

	Point sp = *is, ep = *ie, maxp;

	double l = range(sp, ep);

	double max = 0;
	Iter_point max_i;

	int cnt = 0;
	for(Iter_point i = is; i != ie; i++){
		cnt++;
		Point p = *i;
		double  d = get_measure(sp,ep,p, mode); // range(sp, p) * range(ep, p) / l;
		//double  d = range(sp, p) * range(ep, p) / l;
		if(d > max) max = d,  maxp = p, 	max_i = i;;
	}

	if( max > mind && cnt > 5 ){ 	// 점 5개 이내의 거리는 분할하지  안음
		Iter_point npos = pecker_path->insert(pos, maxp);
		npos = _pecker(pecker_path, npos, is, max_i, mind, mode);
		npos = _pecker(pecker_path, npos + 1, max_i, ie, mind, mode);
		return npos;
	}
	return  pos;
}

/**@brief advanced douglas-pecker 로 라인을 단순화한다.  */
Path douglasPeucker(Path ph,double  min_distance, int mode){
	// doublas_pecker 를 사용하여 패스를 단순화
	Path *pecker_path = new Path;

	Iter_point i_end = ph.end();
	--i_end;

	pecker_path->push_back(*ph.begin());
	pecker_path->push_back(*i_end);

	_pecker( pecker_path, --pecker_path->end(), ph.begin(), i_end, min_distance, mode);

	return *pecker_path;
}


} /* namespace filmInspector */
