/*
Otsu���ڸ������ɱ�����ֵ������ֵ�㷨
Surendra���ڵ������±���
���������������ڸ�����ʦ�ֱ�ҵ���
RatioAdjust������ǿ�Աȶ�
*/
#include<cv.h>
#include<highgui.h>
#include<cxcore.h>
#include "macbsvpmss.h"

//ǰ����ȡ����ֵ�㷨
int Otsu(IplImage* src)
{
	int height = src->height;
	int width = src->width;

	/* ����ͳ��ֱ��ͼ */
	float histogram[256] = { 0 };
	for (int i = 0; i < height; i++)
	{
		unsigned char* p = (unsigned char*)src->imageData + src->widthStep * i;
		for (int j = 0; j < width; j++)
		{
			histogram[*p++]++;
		}
	}
	/* ��ֱ��ͼ���й�һ������ */
	int size = height * width;
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = histogram[i] / size;
	}

	/* �������ͼ���ƽ���Ҷ� */
	float avgValue = 0;
	for (int i = 0; i < 256; i++)
	{
		avgValue += i * histogram[i];  //����ͼ���ƽ���Ҷ�
	}

	int threshold;
	float maxVariance = 0;
	float w = 0, u = 0;
	for (int i = 0; i < 256; i++)
	{
		w += histogram[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷȵ�����(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���
		u += i * histogram[i];  // �Ҷ�i ֮ǰ������(0~i)��ƽ���Ҷ�ֵ�� ǰ�����ص�ƽ���Ҷ�ֵ

		float t = avgValue * w - u;
		float variance = t * t / (w * (1 - w));
		if (variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}
	/* �Ľ��ĵط� */
	if (threshold < 30)
		threshold = 30;

	return threshold;
}

// Surendra���������㷨
void SurendraRenew(CvMat* bin, CvMat* frame, CvMat* background, CvMat* background_renew)
{
	/* ����ָ��CvMat��ÿһ������ֵ��ָ�� */
	float* bin_float;
	float* frame_float;
	float* background_float;
	float* background_renew_float;

	int x, y;

	for (y = 0; y < background_renew->rows; y++)
	{
		/* ����ѭ����ָ��ָ��CvMat��ÿһ�еĿ�ͷԪ��λ�� */
		bin_float = (float*)(bin->data.ptr + y * bin->step);
		frame_float = (float*)(frame->data.ptr + y * frame->step);
		background_float = (float*)(background->data.ptr + y * background->step);
		background_renew_float = (float*)(background_renew->data.ptr + y * background_renew->step);

		for (x = 0; x < background_renew->cols; x++)
		{
			/* ��Ԫ��ֵ���бȽ��жϣ����ڱ����Ĳ��ֽ��õ�ǰ֡��Ӧ����ֵ��䣬�����˶�����Ĳ��ֽ���ԭ������Ӧ����ֵ��� */
			if (*(bin_float + x) == 0)
				*(background_renew_float + x) = *(frame_float + x);
			else
				*(background_renew_float + x) = *(background_float + x);
		}
	}
}
//��ǿ�Աȶ�
void RatioAdjust(IplImage* src, IplImage* dst) {
	double minValue;
	double maxValue;
	cvMinMaxLoc(src, &minValue, &maxValue);
	if (maxValue - minValue<1e-10) {
		cvZero(dst);
		return;
	}
	cvConvertScale(src, dst, 255.0 / (maxValue - minValue), -255.0*minValue / (maxValue - minValue));
}
//���ڷ�����Ӿ������ĵĸ�������
center GetCenter(CvRect rect)
{
	center point;
	point.x = rect.x + rect.width / 2;
	point.y = rect.y + rect.height / 2;
	return point;
}

//ÿ�{��һ���ɔz���^����һ��
IplImage* GetCamFrame(CvCapture* capture)
{

	//CvCapture* capture = cvCaptureFromCAM(0);
	IplImage* frame = cvQueryFrame(capture);
	//�]��cvWaitkey()�͕����ز������Ď� 
	cvWaitKey(1);
	//cvReleaseCapture(&capture);
	return frame;
}
