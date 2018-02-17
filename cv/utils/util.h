#ifndef CV_UTIL_H_
#define CV_UTIL_H_
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace srlib {
/******************************** cvt_color **************************/

Mat asBgBlack(Mat src);
Mat asBgWhite(Mat src);
Mat asGray(Mat src);
Mat asBGR(Mat src);

/******************************** display util ************************/

void putMultiLine(Mat img, const string& text, Point org,	int fontFace, double fontScale, Scalar color, int l_height );
Scalar randcolor(int min, int max);

/******************************** Rect ********************************/

#define NULLRECT Rect{0,0,0,0}

Rect 	imgrect(Mat img );
bool 	inRectP(Rect r, Point p);				// 점 p가 r 안에 존재하는가를 확인
bool 	inRectR(Rect br, Rect sr);				// sr 이 br 안에 존재하는 가를 확인

Rect 	interRect(Rect r1, Rect r2);				// 두 영역중에 교차영역을 계산

Rect 	rectsum(Rect r1,Rect r2);				// 두 영역을 포함하는 영역을 계산
Rect 	rectsum(vector <Rect> rects);			//  vector 의  영역을 합산하는 영역을 계산

Rect 	fitRect(Rect r, Rect a_r);				// r 이 a_r 의 영역을 초과할 경우 이를 a_r 안으로 매핑
char*	tostr(Rect r);							// rect 를 문자열로 1,2,2,1




#define SORT_BY_SX	0 	// start x 로 정렬
#define SORT_BY_SY	1	// start y 로 정렬
#define SORT_BY_MX	2 	// x 중점으로 정렬
#define SORT_BY_MY 	3 	// y 중점으로 정렬

vector <Rect> sortRects( vector <Rect> rects,	// rect vector 를 정렬
		int type = SORT_BY_SX, int inc = 1);


/******************************** cvt_color **************************/

Mat asBgBlack(Mat src);
Mat asBgWhite(Mat src);
Mat asGray(Mat src);
Mat asBGR(Mat src);

/******************************** display util ************************/

void putMultiLine(Mat img, const string& text, Point org,	int fontFace, double fontScale, Scalar color, int l_height );
void drawMark(Mat img, Point p1,int size,Scalar color);


/******************************** Rect ********************************/

Rect 	imgrect(Mat img );
bool 	inRectP(Rect r, Point p);				// 점 p가 r 안에 존재하는가를 확인
bool 	inRectR(Rect br, Rect sr);				// sr 이 br 안에 존재하는 가를 확인

Rect 	interRect(Rect r1, Rect r2);				// 두 영역중에 교차영역을 계산

Rect 	rectsum(Rect r1,Rect r2);				// 두 영역을 포함하는 영역을 계산
Rect 	rectsum(vector <Rect> rects);			//  vector 의  영역을 합산하는 영역을 계산

Rect 	fitRect(Rect r, Rect a_r);				// r 이 a_r 의 영역을 초과할 경우 이를 a_r 안으로 매핑

char 	*tostr(Rect r);							// rect 를 문자열로 1,2,2,1


/********************************** angle ****************************/

double 	lag3Angle(Point p1, Point p2, Point p3, float t);			// 라그랑쥬 공식을 이용해 p1 , p2, p3 를 지나는 곡선의 t( 0~2 ) 시점에서 각도를 구함
double 	line2theta(Point p1, Point p2);								// p1과  p2 가 잇는 line 의 각도를 계산


/********************************** string ****************************/
// 한글처리
String 	wcharAt(String s, int idx);			// 한글여부를 고려해 n번째 글자 리턴
int 		wcharcnt(String s);					// 몇글자로 구성된 스트링인지 리턴


} // namespace srlib
#endif
