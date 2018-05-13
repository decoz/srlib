/*
 * dsRand.h
 *
 *  Created on: 2016. 5. 23.
 *      Author: decoz
 */
#ifndef DSRAND_H_
#define DSRAND_H_

#include <stdio.h>
namespace srlib {
class rnd {
private:
	rnd();

public:

	static rnd *it;

	static rnd* getIt(){
		if(it == NULL)
			it = new rnd();
		return it;
	}

	static int *nrand(int max, int n){ rnd::getIt()->_nrand(max,n);  };	// max 까지의 정수중에 n개의 정수를 랜덤으로 생성
	int *		_nrand(int max, int n);
	virtual ~rnd();
};

}
#endif /* DSRAND_H_ */
