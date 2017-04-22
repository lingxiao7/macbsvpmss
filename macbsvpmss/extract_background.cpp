/*
���㱳���ĺ��������������Ը�����ʦ�ֵı�ҵ��ƴ���
���㹫ʽ��backImage = (1-learningRate)*background + learningRate*gray;
*/
#include "macbsvpmss.h"

//input[]������Ƶ�ľ���·�����ַ���,start��ʾ��ȡ��ʼ֡��end��ʾ������ȡ��ֹ֡
IplImage* ExtractBackground(char input[], int start, int end)
{

	/**********************************************************

	ʵ�ֹ��ܣ�����Ƶ�н�ȡ�����Ķ�֡ת��Ϊ�Ҷ�ͼ��Ȼ����ƽ��ֵ

	*/
	char savepath[] = ".\\";
	CvCapture* capture = cvCreateFileCapture(input);	//������Ƶ������ǰ�뱣֤��·���´��ڸ����Ƶ���Ƶ������ᱨ��

	IplImage* frame = NULL;
	IplImage* frame_gray = NULL;

	CvMat* frame_grayMat = NULL;
	CvMat* img_avgMat = NULL;
	//����ȡ��֡����ʼ����ֹλ�ã������趨������ƽ����֡���ϴ�ʱ��������ʱ����ܻ�ϳ�
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, start);	//����Ƶ֡��λ�õ��������λ��
	int count = start;

	while (cvGrabFrame(capture) && (count <= end))
	{
		frame = cvRetrieveFrame(capture);	//��ȡ��ǰ֡

		if (frame_gray == NULL)
		{
			frame_gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
			cvZero(frame_gray);
		}
		cvCvtColor(frame, frame_gray, CV_BGR2GRAY);		//����ǰ֡ת�ɻҶ�ͼ

		if (frame_grayMat == NULL)
		{
			frame_grayMat = cvCreateMat(frame->height, frame->width, CV_32FC1);
			cvZero(frame_grayMat);
		}
		cvConvert(frame_gray, frame_grayMat);		//���Ҷ�ͼת��Ϊ32λ������Ԫ�صľ����ʽ�Է������

		if (img_avgMat == NULL)
		{
			img_avgMat = cvCreateMat(frame->height, frame->width, CV_32FC1);
			cvZero(img_avgMat);
		}
		float learning = 1.0 / (float)(end - start + 1);
		cvAddWeighted(img_avgMat, 1 - learning, frame_grayMat, learning, 0, img_avgMat);	//���Ҷ�ͼ��ƽ������Ȩֵ���е��ӣ�����ƽ��

		count++;

	}

	IplImage* img_avg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);	//���Ҷ�ͼ�Ӿ����ʽת��ͼ���ʽ
	cvConvert(img_avgMat, img_avg);
	cvReleaseCapture(&capture);
	cvDestroyAllWindows();
	RatioAdjust(img_avg, img_avg);
	//����ûҶ�ͼ 
	char savename[50];
	sprintf(savename, "%s%d%s%d%s", savepath, start, "_to_", end, ".png");
	cvSaveImage(savename, img_avg);
	return img_avg;
}
