/*
 * srrand.cpp
 *
 *  Created on: 2018. 2. 16.
 *      Author: decoz
 */

#include "srrand.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

namespace srlib {
srrand *srrand::it = NULL;

srrand::srrand() {
	// TODO Auto-generated constructor stub
	srand(time(NULL));
}

srrand::~srrand() {
	// TODO Auto-generated destructor stub
}

double srrand::_gauss(double average, double stdev){

	  double v1, v2, s, temp;

	  do {

	     v1 =  2 *  ( (double) rand() / RAND_MAX ) - 1;      // -1.0 ~ 1.0 까지의 값
	     v2 =  2 *  ( (double) rand() / RAND_MAX ) - 1;      // -1.0 ~ 1.0 까지의 값

	     s = v1 * v1 + v2 * v2;
	     /*
	     printf("v1:%.2f, v2:%.2f, s:%.2f \n", v1, v2, s);
	     fflush(stdout);
	     */
	   } while (s >= 1 || s == 0);

	   s = sqrt( (-2 * log(s)) / s );

	   temp = v1 * s;
	   temp = (stdev * temp) + average;

	   return temp;

}
} /* namespace srlib */
