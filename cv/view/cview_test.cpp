/*
 * test_view.cpp
 *
 *  Created on: 2017. 2. 17.
 *      Author: decoz
 */

#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "cview.h"

using namespace cv;
using namespace std;
using namespace srlib;


void handle_rectdrag( Mat img, void *ptr){
	property *pp = (property*)ptr;
	Rect r = pp->dragrect;

	//printf("!!!\n");

	if(r.width * r.height > 25 ) {
		printf("read rect %d,%d,%d,%d done!\n", r.x, r.y, r.width, r.height);;
		cout << "input label:";;
		string label("");
		cin >> label;
		cout << "label is " << label;
	}
}


int main( int argc, char **argv ){
	printf("start cview_test\n");

	for(int i=1;i<argc;i++){
		Mat img = imread(argv[1] /*,CV_LOAD_IMAGE_GRAYSCALE */);


		cview::turnon("cview", "xyhistogram");
		//cview::turnon("cview", "xyposition");

		cview::handle("cview", "evt_dragrect", handle_rectdrag);
		cview::show("cview", img);

		fflush(stdout);
		if( waitKey(0) == 'q') break;
	}
	return  0;

}


