/*
 * serial.h
 *
 *  Created on: 2018. 2. 15.
 *      Author: decoz
 *
 *   managing statistic serial data
 *   and create draw image
 *
 */

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


#ifndef STAT_SERIAL_H_
#define STAT_SERIAL_H_

namespace srlib {

class serial {

	typedef vector<float>::iterator f_i;

private:
	vector <float> values;
	double sum;


	float min, max;
public:
	float 	average();

	float 	represent();											// represent  value
	void 	print();
	void 	put(float v);											// put data
	void 	drawgraph(Mat m, int range, Scalar color);		// draw graph

	serial();
	virtual ~serial();
};

} /* namespace srlib */

#endif /* STAT_SERIAL_H_ */
