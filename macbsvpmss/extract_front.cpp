/*
计算差分图像并二值化的函数，修改自高云晖师兄的毕业设计代码
该函数暂时无法处理大视频
*/
#include "macbsvpmss.h"

void ExtraFront(IplImage* frame, IplImage* front_bin, IplImage* background) {

	// 创建存储矩阵格式的背景灰度图的变量并将初始背景转化为矩阵格式 
	CvMat* background_grayMat = cvCreateMat(background->height, background->width, CV_32FC1);
	cvConvert(background, background_grayMat);
	// 创建与原背景进行加权叠加的背景更新图 
	CvMat* background_renewMat = cvCreateMat(background->height, background->width, CV_32FC1);
	// 创建存储当前帧及其灰度图的变量 
	IplImage* frame_gray = NULL;
	// 创建中值滤波后的当前帧灰度图以及存储其矩阵格式的变量 
	IplImage* frame_median = NULL;
	CvMat* frame_medianMat = NULL;
	// 创建存储前景的变量 
	IplImage* front = NULL;
	CvMat* frontMat = NULL;
	// 创建存储二值化后的前景的变量 
	CvMat* front_binMat = NULL;
	// 创建显示输入以及输出的视频的窗口 

	if (!frame)
		return;
	// 将当前帧转化为灰度图	
	if (frame_gray == NULL)
		frame_gray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	cvCvtColor(frame, frame_gray, CV_BGR2GRAY);
	// 对当前帧进行中值滤波 
	if (frame_median == NULL)
	{
		frame_median = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		frame_medianMat = cvCreateMat(frame->height, frame->width, CV_32FC1);
	}
	CvSize size = cvSize(frame->width, frame->height); // get current frame size，得到当前帧的尺寸 
	cvSmooth(frame_gray, frame_median, CV_MEDIAN);	//默认为3*3的掩膜
													/*float k[9] = {0,-1,0,-1,5,-1,0,-1,0};
													CvMat km = cvMat(3,3,CV_32FC1,k);
													cvFilter2D(frame_median,frame_median,&km);*/
	IplImage*pyr = cvCreateImage(cvSize((size.width & -2) / 2, (size.height & -2) / 2), 8, 1);
	cvPyrDown(frame_median, pyr, CV_GAUSSIAN_5x5);// 向下采样，去掉噪声，图像是原图像的四分之一 
												  //cvDilate( pyr, pyr, 0, 1 ); // 做膨胀操作，消除目标的不连续空洞 
	cvPyrUp(pyr, frame_median, CV_GAUSSIAN_5x5);// 向上采样，恢复图像，图像是原图像的四倍 

	cvConvert(frame_median, frame_medianMat);
	// 进行减景操作得到前景 
	if (front == NULL)
	{
		front = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		frontMat = cvCreateMat(frame->height, frame->width, CV_32FC1);
	}
	cvAbsDiff(frame_medianMat, background_grayMat, frontMat);
	cvConvert(frontMat, front);
	// 对前景进行二值化，算法为改进的OTSU 
	if (front_binMat == NULL)
	{
		front_binMat = cvCreateMat(frame->height, frame->width, CV_32FC1);
	}
	int threshold = Otsu(front);
	//printf("\n*threshold:%d*\n",threshold);
	cvThreshold(front, front_bin, threshold, 255, CV_THRESH_BINARY);
	// 对二值化后的前景做开运算 
	cvErode(front_bin, front_bin);	//腐蚀,迭代次数1
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

