/*
 * serial_test.cpp
 *
 *  Created on: 2018. 2. 15.
 *      Author: decoz
 */

#include "serial.h"
#include "srrand.h"
#include <time.h>
#include <stdio.h>


using namespace cv;
using namespace srlib;

float make_rand(){
	return rand() % 100;
}

float make_grand(){
	return srrand::gauss(50, 10);
}

float make_noise(float noise_rate){
	int nr = noise_rate * 100;

	if(rand() % 100 < nr ) return rand() % 100;
	else return srrand::gauss(50, 5);
}

void	test_graph();
void 	test_filter();

int main() {

	printf("start serial class test\n");
	//test_graph();
	test_filter();
	return 0;
}

void test_filter(){

	serial s;
	for(int i=0;i<50;i++){
		int r = make_noise(0.1);
		s.put( (float) r);
	}

	s.represent();
	s.print();
}

void  test_graph(){

	srand(time(NULL));

	Mat m(800,800,CV_8UC3);
	serial s;

	for(int ri=0;  ri<100; ri++){
		m.setTo(Scalar(255,255,255));

		for(int i=0;i<3;i++){
			int r = make_noise(0.02);
			s.put( (float) r);
		}

		Mat gm = m(Rect(50,50,400,100));
		s.drawgraph(gm ,200,Scalar(0,0,0));

		imshow("m", m);
		if( waitKey(100) == 'q') break;;
	}
}

