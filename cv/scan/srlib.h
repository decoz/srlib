/**
 * @brief subrain library
 */

#ifndef SRLIB_H_
#define SRLIB_H_

#include <opencv2/opencv.hpp>

namespace srlib {
using namespace cv;

inline float range(Point p1, Point p2 );
double	line2ang(Point p1, Point p2);
double 	peppLength(Point sp, Point ep, Point pp);
double 	peppInLength(Point sp, Point ep, Point pp);
Vec2f 		theta2vec(double theta);

}

#endif /* SRLIB_H_ */
