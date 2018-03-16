/*
 * direction.cpp
 *
 *  Created on: 2018. 3. 15.
 *      Author: decoz
 *
 *  utility for access surrounding pixels for pixel automata process
 *
 */
#include "util.h"

/** @brief create neighbor pixel array from -1,-1  to -1,0 */
Point *createDirArr(int r){
	int x = -1 * r, y = -1 * r;
	Point *d = new Point[r * 8];
	int w = r * 2;

	for(int t=0;t<8*r;t++){
		d[t] = Point(x,y);

		int dx = ((  t / w + 1) % 2 ) * (1 - ( t / r / 4 ) * 2);
		int dy = (  t / w ) % 2 * (1 - ( t / r / 4 ) * 2);
		x += dx;
		y += dy;
	}

	return d;
}



