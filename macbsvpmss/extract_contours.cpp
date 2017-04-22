/*
提取轮廓并用矩形圈出目标轮廓的函数
该函数有待扩展为连续帧轮廓的追踪
*/
#include "macbsvpmss.h"

using namespace std;
//矩形过滤面积
const int CONTOUR_MAX_AERA = 200;


//test_gray表示需要寻找轮廓的灰度图
//out表示画好矩形的输出图
void ExtractContours(IplImage* test_gray, IplImage* out, vector<tracedata>& t_rect)
{
	vector<tracedata> tmp_rect;
	CvMemStorage *ms = cvCreateMemStorage();
	CvSeq *seq = NULL;
	/*cvNamedWindow("test");
	cvShowImage("test",test_gray);
	cvWaitKey(0);*/

	int cnt = cvFindContours(test_gray, ms, &seq, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (; seq; seq = seq->h_next)
	{
		int c = seq->total;//当前轮廓包含多少个元素，这里的元素为点
		double length = cvArcLength(seq); //得到指定的那个轮廓的周长
										  //该函数有3个参数：序列，起点（默认计算整条曲线），是否封闭曲线
		double area = cvContourArea(seq);  //得到指定的那个轮廓的面积
		CvRect rect = cvBoundingRect(seq, 1);  //根据序列，返回轮廓外围矩形；
		CvBox2D box = cvMinAreaRect2(seq, NULL); //最小外围矩形 

												 /*cout<<"Length = "<<length<<endl;
												 cout<<"Area = "<<area<<endl; */
		CvRect r = ((CvContour*)seq)->rect;
		if (r.height * r.width > CONTOUR_MAX_AERA) // 面积小的方形抛弃掉 
		{

			cvRectangle(out, cvPoint(r.x, r.y),
				cvPoint(r.x + r.width, r.y + r.height),
				CV_RGB(255, 0, 0), 1, CV_AA, 0);
			tracedata tmp;
			tmp.rect = r;
			tmp.num = 1;
			tmp_rect.push_back(tmp);

		}
		/*cout<<"r.x:="<<r.x<<" r.y:="<<r.y<<endl;*/

	}
	UpdateContour(tmp_rect, t_rect);
	char buf[32];
	CvFont font;

	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 1, 2, 8);
	for (vector<tracedata>::iterator it = t_rect.begin(); it != t_rect.end(); it++) {
		if (it->exist == false)
		{
			continue;
		}
		cvPutText(out, _itoa(it->id, buf, 10), cvPoint(GetCenter(it->rect).x, GetCenter(it->rect).y), &font, CV_RGB(255, 0, 0));
		//cv::putText( out, it->id, Point(GetCenter(it->rect).x,GetCenter(it->rect).y),CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0) ); 
		cout << "center:" << GetCenter(it->rect).x << " , " << GetCenter(it->rect).y << "id:" << it->id << endl;
	}
	cvReleaseMemStorage(&ms);
}
