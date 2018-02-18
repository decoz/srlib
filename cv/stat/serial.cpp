/*
 * serial.cpp
 *
 *  Created on: 2018. 2. 15.
 *      Author: decoz
 */

#include "serial.h"

namespace srlib {

serial::serial() {
	sum = 0;

}

serial::~serial() {
	// TODO Auto-generated destructor stub
}


void serial::put(float v){
	if( !values.size() || min > v ) min = v;
	if( !values.size() || max < v ) max = v;
	sum += v;
	values.push_back(v);
}

void serial::print(){
	for(f_i i=values.begin(); i != values.end();  i++)
		printf("%.2f\n", *i);
}

float serial::average(){
	return sum / values.size();
}

/** @brief create representative value from max density */
float serial::represent(){
	int n = values.size();

	double range = max - min;

	vector <float> sorted;
	int best;
	if( n > 0 ){
		sorted.assign(values.begin(),  values.end() );
		sort(sorted.begin(), sorted.end());

		float density[ n ];

		int ci = 0;
		for(f_i i=sorted.begin(); i != sorted.end();  i++){
			double rsum = 0;
			for(f_i j=sorted.begin()+1; j != sorted.end();  j++){
				if( i == j ) continue;
				double r = (*i - *j);
				rsum += r*r;
			}
			density[ci++] = range/rsum;
		}

		double max = 0;

		for(int i=0; i<n; i++){
			if( density[i] > max ) max = density[i], best = i;
		}

		return sorted[best];

	} else return 0;



}

void serial::drawgraph(Mat m, int range = 0, Scalar color = Scalar(255,255,255)){


	int w = m.cols, h  = m.rows;

	if( !values.size() ) return ;

	float padding = w / range;

	if( !range ) range = values.size();

	f_i i;
	if( (unsigned) range >  values.size() ) i = values.begin();
	else i = values.end() - range;

	int x =  0, lasty = -1;
	for(; i != values.end(); i++) {
		int y = ( *i - min ) * h / ( max - min );
		if( lasty >= 0 ) 	line(m, Point(x-padding, lasty), Point(x, y), color);
		lasty = y, x = x + padding;
	}
}

} /* namespace srlib */
