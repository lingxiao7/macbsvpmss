#include "macbsvpmss.h"
#include "opencv2/core/core.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
void GmmExtraFront(IplImage* frame, IplImage* front_bin) {
	IplImage* background;
	cv::BackgroundSubtractorMOG2 mog(530, 4, true);
	cv::Mat mat_gray(frame);
	cv::Mat mat_front(front_bin);
	IplImage* front;
	mog(mat_gray, mat_front, 0.01);
	IplImage tmp = mat_front;
	front = cvCloneImage(&tmp);
	double thresh = Otsu(front_bin);
	cvThreshold(front, front_bin, thresh, 255, CV_THRESH_BINARY);
	// 对二值化后的前景做开运算 
	cvReleaseImage(&front);
	cvDilate(front_bin, front_bin);
	cvNamedWindow("front");
	cvShowImage("front", front_bin);
	/*cvNamedWindow("front");
	cvShowImage("front",front_bin);*/
}
void GmmExtraFront(IplImage* frame, IplImage* front_bin, IplImage* background) {
	cv::Mat backgroundMat;
	cv::BackgroundSubtractorMOG mog(530, 4, true);
	cv::Mat mat_gray(frame);
	cv::Mat mat_front(front_bin);
	mog(mat_gray, mat_front, 0.01);
	mog.getBackgroundImage(backgroundMat);
	IplImage tmp = mat_front;
	//background = cvCloneImage(&tmp);
	front_bin = cvCloneImage(&tmp);
	// 对二值化后的前景做开运算 
	cvNamedWindow("front");
	cvShowImage("front", front_bin);
}
