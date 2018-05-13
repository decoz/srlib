/*
 * dsRand.cpp
 *
 *  Created on: 2016. 5. 23.
 *      Author: decoz
 */

#include <time.h>
#include <stdlib.h>
#include "rnd.h"

namespace srlib {

rnd *rnd::it = NULL;

rnd::rnd() {
	// TODO Auto-generated constructor stub
	srand(time(NULL));
	//printf("--srand--\n"); fflush(stdout);
}

rnd::~rnd() {
	// TODO Auto-generated destructor stub
}


int *rnd::_nrand(int max, int n){
/*
 * 0~max 까지의 숫자중 n개의 랜덤넘버를 배열로 생성
 */

	int *vals = new int[n], map[max] = {0,};

	for(int i=0;i<n;i++){
		int v =  rand() % (max - i), sv = 0;

		while( sv <= v ) {
			if( map[sv++]  ) v++;
		}
		map[v] = 1;
		vals[i] = v;
	}

	return vals;

}
}
