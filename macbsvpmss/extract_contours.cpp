/*
��ȡ�������þ���Ȧ��Ŀ�������ĺ���
�ú����д���չΪ����֡������׷��
*/
#include "macbsvpmss.h"

using namespace std;
//���ι������
const int CONTOUR_MAX_AERA = 200;


//test_gray��ʾ��ҪѰ�������ĻҶ�ͼ
//out��ʾ���þ��ε����ͼ
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
		int c = seq->total;//��ǰ�����������ٸ�Ԫ�أ������Ԫ��Ϊ��
		double length = cvArcLength(seq); //�õ�ָ�����Ǹ��������ܳ�
										  //�ú�����3�����������У���㣨Ĭ�ϼ����������ߣ����Ƿ�������
		double area = cvContourArea(seq);  //�õ�ָ�����Ǹ����������
		CvRect rect = cvBoundingRect(seq, 1);  //�������У�����������Χ���Σ�
		CvBox2D box = cvMinAreaRect2(seq, NULL); //��С��Χ���� 

												 /*cout<<"Length = "<<length<<endl;
												 cout<<"Area = "<<area<<endl; */
		CvRect r = ((CvContour*)seq)->rect;
		if (r.height * r.width > CONTOUR_MAX_AERA) // ���С�ķ��������� 
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
