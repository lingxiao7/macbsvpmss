/*
������ͼ�񲢶�ֵ���ĺ������޸��Ը�����ʦ�ֵı�ҵ��ƴ���
�ú�����ʱ�޷��������Ƶ
*/
#include "macbsvpmss.h"

void ExtraFront(IplImage* frame, IplImage* front_bin, IplImage* background) {

	// �����洢�����ʽ�ı����Ҷ�ͼ�ı���������ʼ����ת��Ϊ�����ʽ 
	CvMat* background_grayMat = cvCreateMat(background->height, background->width, CV_32FC1);
	cvConvert(background, background_grayMat);
	// ������ԭ�������м�Ȩ���ӵı�������ͼ 
	CvMat* background_renewMat = cvCreateMat(background->height, background->width, CV_32FC1);
	// �����洢��ǰ֡����Ҷ�ͼ�ı��� 
	IplImage* frame_gray = NULL;
	// ������ֵ�˲���ĵ�ǰ֡�Ҷ�ͼ�Լ��洢������ʽ�ı��� 
	IplImage* frame_median = NULL;
	CvMat* frame_medianMat = NULL;
	// �����洢ǰ���ı��� 
	IplImage* front = NULL;
	CvMat* frontMat = NULL;
	// �����洢��ֵ�����ǰ���ı��� 
	CvMat* front_binMat = NULL;
	// ������ʾ�����Լ��������Ƶ�Ĵ��� 

	if (!frame)
		return;
	// ����ǰ֡ת��Ϊ�Ҷ�ͼ	
	if (frame_gray == NULL)
		frame_gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	cvCvtColor(frame, frame_gray, CV_BGR2GRAY);
	// �Ե�ǰ֡������ֵ�˲� 
	if (frame_median == NULL)
	{
		frame_median = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		frame_medianMat = cvCreateMat(frame->height, frame->width, CV_32FC1);
	}
	CvSize size = cvSize(frame->width, frame->height); // get current frame size���õ���ǰ֡�ĳߴ� 
	cvSmooth(frame_gray, frame_median, CV_MEDIAN);	//Ĭ��Ϊ3*3����Ĥ
													/*float k[9] = {0,-1,0,-1,5,-1,0,-1,0};
													CvMat km = cvMat(3,3,CV_32FC1,k);
													cvFilter2D(frame_median,frame_median,&km);*/
	IplImage*pyr = cvCreateImage(cvSize((size.width & -2) / 2, (size.height & -2) / 2), 8, 1);
	cvPyrDown(frame_median, pyr, CV_GAUSSIAN_5x5);// ���²�����ȥ��������ͼ����ԭͼ����ķ�֮һ 
												  //cvDilate( pyr, pyr, 0, 1 ); // �����Ͳ���������Ŀ��Ĳ������ն� 
	cvPyrUp(pyr, frame_median, CV_GAUSSIAN_5x5);// ���ϲ������ָ�ͼ��ͼ����ԭͼ����ı� 

	cvConvert(frame_median, frame_medianMat);
	// ���м��������õ�ǰ�� 
	if (front == NULL)
	{
		front = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		frontMat = cvCreateMat(frame->height, frame->width, CV_32FC1);
	}
	cvAbsDiff(frame_medianMat, background_grayMat, frontMat);
	cvConvert(frontMat, front);
	// ��ǰ�����ж�ֵ�����㷨Ϊ�Ľ���OTSU 
	if (front_binMat == NULL)
	{
		front_binMat = cvCreateMat(frame->height, frame->width, CV_32FC1);
	}
	int threshold = Otsu(front);
	//printf("\n*threshold:%d*\n",threshold);
	cvThreshold(front, front_bin, threshold, 255, CV_THRESH_BINARY);
	// �Զ�ֵ�����ǰ���������� 
	cvErode(front_bin, front_bin);	//��ʴ,��������1
	cvSmooth(front_bin, front_bin, CV_MEDIAN, 3, 0, 0, 0);
	cvConvert(front_bin, front_binMat);
	cvReleaseMat(&background_grayMat);
	cvReleaseMat(&background_renewMat);
	cvReleaseImage(&frame_gray);
	cvReleaseImage(&frame_median);
	cvReleaseMat(&frame_medianMat);
	cvReleaseImage(&front);
	cvReleaseMat(&frontMat);
	cvReleaseMat(&front_binMat);
}

