/*
 * srrand_test.cpp
 *
 *  Created on: 2018. 2. 16.
 *      Author: decoz
 */

#include "srrand.h"
#include <stdio.h>
using namespace srlib;

int  main(){
	printf("srrand test \n");
	for(int i=0; i<10; i++){
		printf("%d\n", int( srrand::gauss(50,3) ) );
	}
	return 1;
}
