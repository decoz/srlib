#include <opencv2/opencv.hpp>
#include "srlib.h"
namespace srlib {
using namespace cv;

double line2ang(Point p1, Point p2){
	double ang1 = atan2( double(p2.y - p1.y), double(p2.x - p1.x)) * 180 / CV_PI;
	return ang1;
}


/** @brief �� ���� �Ÿ��� ���Ѵ� */
inline float range(Point p1, Point p2 ) {
	return sqrt ( (float) ( p1.x - p2.x ) * ( p1.x - p2.x ) + ( p1.y - p2.y ) * ( p1.y - p2.y ) );
}



/**@brief �� pp ���� sp - ep �� ����ϴ� ���������� �Ÿ��� ��������
 * ���� sp,ep�� ���� �ۿ� ������ -1�� ����  */
double peppInLength(Point sp, Point ep, Point pp){

	double ang1 = line2ang(sp,ep) * CV_PI / 180 ;
	double ang2 = line2ang(sp,pp) * CV_PI / 180 ;
	double ang =   line2ang(sp,ep) - line2ang(sp,pp);

	float r = range(sp,ep), r1 = range(pp, sp), r2 = range(pp, ep);


	if( abs( r1*r1 - r2*r2 ) > r*r ) return -1;
	else return sin(ang1 - ang2) * r1;
}

Vec2f theta2vec(double theta){
	float vx = 0, vy = 0;

	if(  abs(theta) < CV_PI / 4 || abs(theta) >  CV_PI * 3/4  ){
		vx =  cos(theta) > 0 ? 1 : -1;
		vy = sin(theta) / cos(theta) * vx;
	} else {
		vy =  sin(theta) > 0 ? 1 : -1;
		vx =  cos(theta) / sin(theta) * vy;
	}

	return Vec2f(vx, vy);
}

/**
 * @brief �� rect �� ������ rect�ΰ� �˻�
 */
inline  bool equal_rect(Rect r1, Rect r2){
	if( r1.x == r2.x && r1.y == r2.y
		&& r1.width == r2.width
		&& r1.height == r2.height
	) return true;
	else return false;
}

}
