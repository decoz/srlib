/*
 * peucker.h
 *
 *  Created on: 2017. 1. 13.
 *      Author: decoz
 */

#ifndef PEUCKER_H_
#define PEUCKER_H_

#include <opencv2/opencv.hpp>
//#include "cv_util/cv_util.h"

namespace srlib {
using namespace cv;
using namespace std;

enum peucker_mode {
	MODE_DP,
	MODE_ADP
};

typedef vector <Point> Path;
typedef vector <Path>::iterator Iter_path;
typedef vector <Point>::iterator Iter_point;

Path 	douglasPeucker(Path ph,double  min_distance, int mode = MODE_DP);




} /* namespace filmInspector */

#endif /* PEUCKER_H_ */
