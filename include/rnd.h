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

	static int *nrand(int max, int n){ rnd::getIt()->_nrand(max,n);  };	// max ������ �����߿� n���� ������ �������� ����
	int *		_nrand(int max, int n);
	virtual ~rnd();
};

}
#endif /* DSRAND_H_ */
