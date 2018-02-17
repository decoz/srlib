#include "util.h"

namespace srlib {
double lag3Angle(Point p1, Point p2, Point p3, float t){
/*
 * p1 , p2, p3 �� ������ ��׶��� ���
 * t �������� ���⺤�͸� Point ������ �����Ѵ�..
 * t = 0 �ΰ�� ������, t = 2 �ΰ�� ������ ���Ⱑ ���ϵȴ�..
 */
	float ax = float( p1.x ) - float( 2 * p2.x) + float( p3.x );
	float ay = float( p1.y ) - float( 2 * p2.y) + float( p3.y );

	float bx = float( -3 * p1.x / 2 ) + float( p2.x * 2 ) + float( -1 * p3.x / 2 );
	float by = float( -3 * p1.y / 2 ) + float( p2.y * 2 ) + float( -1 * p3.y / 2 );

	Point v = Point( (ax * t + bx)*10 ,( ay * t + by) * 10);

	return line2theta(Point(0,0), v);

}

double line2theta(Point p1, Point p2){
	double ang1 = atan2( double(p2.y - p1.y), double(p2.x - p1.x)) * 180 / CV_PI;
	return ang1;
}
} // namespace  srlib


