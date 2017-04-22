/*���¸��ٵ��е�������Ӿ��Σ�ǰ��֡��ͬ����¸���֡����
�¾��μ��뵽����ĩβ,src����һ֡�ľ��μ��ϣ�des�Ǹ��ٵ��еľ��μ���
*/
#include "macbsvpmss.h"
#include <cmath>
#include<fstream>
#include<string>
#include<cstdlib>

using namespace std;
//�Զ�����������

void UpdateContour(vector<tracedata>& src, vector<tracedata>& des)
{
	string cord;
	int filenum = 0;
	const double thresh = 200;
	bool flag;
	for (vector<tracedata>::iterator it = des.begin(); it != des.end(); ++it)
	{
		it->exist = false;
	}
	for (vector<tracedata>::iterator itSrc = src.begin(); itSrc != src.end(); itSrc++)
	{
		flag = false;
		for (vector<tracedata>::iterator itDes = des.begin(); itDes != des.end(); itDes++)
		{
			if (fabs(GetCenter(itSrc->rect).x - GetCenter(itDes->rect).x) < thresh)
			{
				if (fabs(GetCenter(itSrc->rect).y - GetCenter(itDes->rect).y) < thresh)
				{
					char buf[32];
					//string temp=_itoa(filenum,buf,10);
					itDes->exist = true;
					itDes->num++;
					itDes->rect = itSrc->rect;
					flag = true;
					/*string filename = "F:\\filename\\"+temp+".txt";
					filenum++;
					ofstream out(filename.c_str(),ios::out);
					if(!out)
					{
					//cout<<"filename can not open"<<endl;

					}
					out<<itSrc->rect.x<<" "<<itSrc->rect.y<<" "<<itSrc->rect.width<<" "<<itSrc->rect.height<<endl;


					out.close();*/
					break;
				}
			}
		}
		if (!flag)
		{
			itSrc->id = des.size();
			des.push_back(*itSrc);
		}
	}
}
