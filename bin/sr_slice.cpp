/*
 * sr_slice.cpp
 *
 *  Created on: 2017. 2. 23.
 *      Author: decoz
 *
 *  이미지를 n x m 으로 슬라이스 하는 프로그램
 *
 *  사용예 :
 *
 *  	sr_slice 4x3 *.jpg
 *
 *
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <sys/fileloader.h>

using namespace cv;

int main(int argc, char **argv) {

	fileloader fl;
	char*		fname;
	int			cols = 0, rows = 0;
	char 		buff[100];

	if(argc>1) sscanf(argv[1], "%dx%d",  &cols, &rows);

	if( ! (cols*rows) ) {
		printf("usage :  sr_slice [n]x[m] [filename] ");
		printf("  ex : sr_slice 5x4 *.jpg ");
		return 0;
	}

	for(int i=2; i<argc ;i++){
		printf("%s\n",argv[i]);
		fl.load(argv[i]);
	}

	while( ( fname = fl.next() ) != NULL) {
		printf("load: %s\n", fname);
		Mat img = imread(fname);

		int w = img.cols / cols, h = img.rows / rows;

		if( img.data ){
			for(int r=0;r<rows;r++) for(int c=0;c<cols;c++){
				char *fname_head =  strtok(fname, ".");

				sprintf( buff, "%s_r%02dc%02d.png", fname_head, r, c);
				imwrite(buff, img( Rect( c * w, r * h, w, h )) );
				printf("  %s\n", buff);
			}
		} else printf("error: not a valid image %s\n", fname);
	}

	return 0;
}



