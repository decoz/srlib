#include "util.h"

using namespace cv;
namespace srlib {

Mat asBGR(Mat src){
	Mat bgr;
	if(src.channels() < 3){
		cvtColor(src,bgr,CV_GRAY2BGR);
	} else {
		bgr = src;
	}

	return bgr;
}

Mat asGray(Mat src){
	Mat gray;
	if(src.channels() >= 3){
		cvtColor(src,gray,CV_BGR2GRAY);
	} else {
		gray = src;
	}

	return gray;

}

Mat asBgBlack(Mat gray){
	if( sum(gray)[0] > 127 * gray.rows * gray.cols ) bitwise_not(gray,gray);
	return gray;
}

Mat asBgWhite(Mat gray){
	if( sum(gray)[0] < 127 * gray.rows * gray.cols ) bitwise_not(gray,gray);
	return gray;
}
} // namespace srlib
