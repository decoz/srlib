/*
 * srrand.h
 *
 *  Created on: 2018. 2. 16.
 *      Author: decoz
 */

#ifndef STAT_SRRAND_H_
#define STAT_SRRAND_H_

#include <stdlib.h>

using namespace std;

namespace srlib {

class srrand {
private:
	srrand();

public:
	static srrand* it;
	static 	srrand* 	getIt(){if(it == NULL) it = new srrand(); return it; }

	static 	double gauss(double average, double stdev) 	{ // gaussian random
		return getIt()->_gauss(average, stdev);
	};

	double _gauss(double average, double stdev);
	virtual ~srrand();
};

} /* namespace srlib */

#endif /* STAT_SRRAND_H_ */
