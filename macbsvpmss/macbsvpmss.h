#pragma once
#ifndef GUARD_macbsvpmss_h
#define GUARD_macbsvpmss_h

#include<cxcore.h>
#include<highgui.h>
#include<cvaux.h>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include <list>
#include <vector>
struct tracedata
{
	CvRect rect;
	long num;
	int id;
	bool exist;
};
struct center {
	double x;
	double y;
};
center GetCenter(CvRect rect);

//������ȡ
IplImage* ExtractBackground(char*, int, int);
//ǰ����ȡ����ֵ��
void ExtraFront(IplImage*, IplImage*, IplImage*);
//ѡ��Ŀ��
void ExtractContours(IplImage*, IplImage*, std::vector<tracedata>&);
//ǰ����ȡ����ֵ�㷨
int Otsu(IplImage*);
//���������㷨
void SurendraRenew(CvMat*, CvMat*, CvMat*, CvMat*);
//��������
void UpdateContour(std::vector<tracedata>&, std::vector<tracedata>&);
//��ǿ�Աȶ�
void RatioAdjust(IplImage*, IplImage*);
//������
void MobileDectect(char*, char*, std::vector<tracedata>&);
void GmmExtraFront(IplImage* frame, IplImage* front_bin);
void GmmExtraFront(IplImage* frame, IplImage* front_bin, IplImage* background);
IplImage* GetCamFrame(CvCapture*);
#endif
