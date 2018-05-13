/*
 * skel.cpp
 *
 *  Created on: 2014. 8. 12.
 *      Author: decoz
 */

#include "skel.h"

Point *createDirArr(int r){
/*
 * range r 의 주변 영역 direction 에 대한 상대좌표 배열값을 만든다.
 * 좌측 상단을 배열의 시작(0) 으로 잡는다.
 *
 * ex) r = 1
 *
 * -1-1 , 0-1, 1-1, 1 0, 1 1, 0 1, -1 1, -1 0
 *
 */
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



inline bool inbound(Mat tmap, int y, int x){
	if( y >= tmap.rows || y < 0 )  return false;
	if( x >= tmap.cols || x < 0 )  return false;
	return  true;
}


skel::skel() {
	// TODO Auto-generated constructor stub
	max_step = -1; // 조건이 맞을때까지 계속
}

skel::~skel() {
	// TODO Auto-generated destructor stub
}



Mat skel::createSkel(Mat bimg){
/*
 * 스켈레톤 이미지를 만든다.
 * 현재는 zhang-suen 기법만 지원
 */

	/*
	if(img.channels() > 1)  cvtColor(img,src,CV_BGR2GRAY);
	else src = img;
	*/

	src = bimg;
	zhThinning();
	return skelimg * 255;

}

void skel::showStep(){

	threshold(src, skelimg, 140, 255, CV_THRESH_BINARY);
	//skel = src.clone();
	skelimg /= 255;

	//int c = 1;
	//bool change = true;
	Mat prev = Mat::zeros(skelimg.size(), CV_8UC1);
	Mat diff;


	Mat step;

	//char buffer[20];
	do {
		//change = false;

		zhIteration(0);
		step = skelimg * 255;
		imshow("skel_step",step);
		waitKey(0);

		zhIteration(1);
		step = skelimg * 255;
		imshow("skel_step",step);
		waitKey(0);
		absdiff(skelimg,prev,diff);
		skelimg.copyTo(prev);


	} while (countNonZero(diff)  > 0);

	skelimg *= 255;

}


void skel::zhThinning(){
	/*
	 * zhang-suen 기법의 thinning 알고리즘
	 */

	threshold(src, skelimg, 140, 255, CV_THRESH_BINARY);
	//skel = src.clone();
	skelimg /= 255;

	//int c = 0;
	//bool change = true;
	Mat prev = Mat::zeros(skelimg.size(), CV_8UC1);
	Mat diff;

	int step_cnt = 0;
	do {
		//change = false;
		zhIteration(0);
		zhIteration(1);
		absdiff(skelimg,prev,diff);
		skelimg.copyTo(prev);

		if( max_step > 0 &&  ++step_cnt > max_step) break;
	} while (countNonZero(diff)  > 0);
}

bool skel::zhIteration(int iter){
	/*
	 * zhang-shuen iteration
	 */
	Mat marker = Mat::zeros(skelimg.size(), CV_8UC1);
	Point *d8 = createDirArr(1);

	//bool change = false;
	//int d[4];
	bool check_bound;
	for (int i = 0; i < skelimg.rows; i++)
	{
		for (int j = 0; j < skelimg.cols; j++)
		{
			if(  skelimg.at<uchar>(i, j) ==  1){

				uchar p[8];
				for(int di=0;di<8;di++){
					Point d =  d8[ (di+1) % 8];
					p[di] = inbound(skelimg,d.y+i, d.x + j )?
							skelimg.at<uchar>( d.y+i, d.x + j ) : 0;
				}

				int A = 0;
				int B = 0;
				for(int t = 0;t<8;t++){
					if((p[t] == 0 && p[(t+1)%8] == 1)){
						//d[A++] = (t+1)%8;
						A++;
					}

					B  += p[t];
				}

				int m1 = iter == 0 ? (p[0] * p[2] * p[4]) : (p[0] * p[2] * p[6]);
				int m2 = iter == 0 ? (p[2] * p[4] * p[6]) : (p[0] * p[4] * p[6]);


				if(A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0){
					marker.at<uchar>(i,j) = 1;
				}
			}
		}
	}

	skelimg &= ~marker;
	return true;
}
