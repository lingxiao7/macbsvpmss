/*
Otsu用于辅助生成背景二值化的阈值算法
Surendra用于迭代更新背景
这两个函数来自于高云晖师兄毕业设计
RatioAdjust用于增强对比度
*/
#include<cv.h>
#include<highgui.h>
#include<cxcore.h>
#include "macbsvpmss.h"

//前景提取的阈值算法
int Otsu(IplImage* src)
{
	int height = src->height;
	int width = src->width;

	/* 建立统计直方图 */
	float histogram[256] = { 0 };
	for (int i = 0; i < height; i++)
	{
		unsigned char* p = (unsigned char*)src->imageData + src->widthStep * i;
		for (int j = 0; j < width; j++)
		{
			histogram[*p++]++;
		}
	}
	/* 对直方图进行归一化处理 */
	int size = height * width;
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = histogram[i] / size;
	}

	/* 求得整幅图像的平均灰度 */
	float avgValue = 0;
	for (int i = 0; i < 256; i++)
	{
		avgValue += i * histogram[i];  //整幅图像的平均灰度
	}

	int threshold;
	float maxVariance = 0;
	float w = 0, u = 0;
	for (int i = 0; i < 256; i++)
	{
		w += histogram[i];  //假设当前灰度i为阈值, 0~i 灰度的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例
		u += i * histogram[i];  // 灰度i 之前的像素(0~i)的平均灰度值： 前景像素的平均灰度值

		float t = avgValue * w - u;
		float variance = t * t / (w * (1 - w));
		if (variance > maxVariance)
		{
			maxVariance = variance;
			threshold = i;
		}
	}
	/* 改进的地方 */
	if (threshold < 30)
		threshold = 30;

	return threshold;
}

// Surendra背景更新算法
void SurendraRenew(CvMat* bin, CvMat* frame, CvMat* background, CvMat* background_renew)
{
	/* 创建指向CvMat中每一个像素值的指针 */
	float* bin_float;
	float* frame_float;
	float* background_float;
	float* background_renew_float;

	int x, y;

	for (y = 0; y < background_renew->rows; y++)
	{
		/* 利用循环令指针指向CvMat中每一行的开头元素位置 */
		bin_float = (float*)(bin->data.ptr + y * bin->step);
		frame_float = (float*)(frame->data.ptr + y * frame->step);
		background_float = (float*)(background->data.ptr + y * background->step);
		background_renew_float = (float*)(background_renew->data.ptr + y * background_renew->step);

		for (x = 0; x < background_renew->cols; x++)
		{
			/* 对元素值进行比较判断，属于背景的部分将用当前帧对应像素值填充，属于运动物体的部分将用原背景对应像素值填充 */
			if (*(bin_float + x) == 0)
				*(background_renew_float + x) = *(frame_float + x);
			else
				*(background_renew_float + x) = *(background_float + x);
		}
	}
}
//增强对比度
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
//用于返回外接矩形中心的辅助函数
center GetCenter(CvRect rect)
{
	center point;
	point.x = rect.x + rect.width / 2;
	point.y = rect.y + rect.height / 2;
	return point;
}

//每調用一次由攝像頭返回一幀
IplImage* GetCamFrame(CvCapture* capture)
{

	//CvCapture* capture = cvCaptureFromCAM(0);
	IplImage* frame = cvQueryFrame(capture);
	//沒了cvWaitkey()就會返回不正常的幀 
	cvWaitKey(1);
	//cvReleaseCapture(&capture);
	return frame;
}
