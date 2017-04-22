#include "macbsvpmss.h"
using namespace std;
void MobileDectect(char video[], char* back_img, vector<tracedata>&t_rect)
{
	int b_start = 20;
	int b_end = 50;
	CvCapture* capture = cvCreateFileCapture(video);
	//static CvCapture* capture = cvCreateCameraCapture(-1);
	//cvSetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES,700000);
	int start = 0;
	int count = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	IplImage* frame = NULL;
	IplImage* frame_gray = NULL;
	IplImage* frame_binary = NULL;
	IplImage* background = NULL;
	//background = ExtractBackground(video,b_start,b_end);
	background = cvLoadImage(back_img, 0);
	cvNamedWindow("background", 1);
	//while(cvGrabFrame(capture) && start < count)
	while (frame = cvQueryFrame(capture))
	{
		frame = cvRetrieveFrame(capture);
		if (NULL == frame_gray)
		{
			frame_gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
			cvZero(frame_gray);
		}
		frame_binary = NULL;
		frame_binary = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, frame_gray, CV_BGR2GRAY);
		//ExtraFront(frame,frame_binary,background);
		GmmExtraFront(frame_gray, frame_binary);
		//GmmExtraFront(frame,frame_binary,background);
		ExtractContours(frame_binary, frame, t_rect);
		cvShowImage("background", frame);
		if (cvWaitKey(10) >= 0)//10ms中按任意键退出 
			break;
		start++;
		cvReleaseImage(&frame_gray);
		cvReleaseImage(&frame_binary);
	}
	cvReleaseCapture(&capture);
	cvReleaseImage(&background);
}
