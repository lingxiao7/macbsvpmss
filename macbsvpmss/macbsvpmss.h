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

//背景提取
IplImage* ExtractBackground(char*, int, int);
//前景提取并二值化
void ExtraFront(IplImage*, IplImage*, IplImage*);
//选中目标
void ExtractContours(IplImage*, IplImage*, std::vector<tracedata>&);
//前景提取的阈值算法
int Otsu(IplImage*);
//背景更新算法
void SurendraRenew(CvMat*, CvMat*, CvMat*, CvMat*);
//轮廓更新
void UpdateContour(std::vector<tracedata>&, std::vector<tracedata>&);
//增强对比度
void RatioAdjust(IplImage*, IplImage*);
//锁定车
void MobileDectect(char*, char*, std::vector<tracedata>&);
void GmmExtraFront(IplImage* frame, IplImage* front_bin);
void GmmExtraFront(IplImage* frame, IplImage* front_bin, IplImage* background);
IplImage* GetCamFrame(CvCapture*);
#endif
