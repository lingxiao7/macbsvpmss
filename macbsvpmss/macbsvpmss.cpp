
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include<cstring>
#include <algorithm>
#include "macbsvpmss.h"
using namespace std;
/*
char video[][100] = {
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\a2.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\t1.avi"
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\789759_6.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790071_6.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790100_5.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790100_8.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790292_5.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\790292_8.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\791745_7.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\791828_5.avi",
	"F:\\vs2005\\carDet\\IPMS-Test-Data\\791828_8.avi",
};
*/
void outPut(vector<tracedata> t_rect)
{
	for (vector<tracedata>::iterator it = t_rect.begin(); it != t_rect.end(); it++)
	{
		cout << "x: " << it->rect.x << " y: " << it->rect.y << " num: " << it->num << " id: " << it->id << endl;
	}
}

//第一个参数是视频路径，第二个参数是背景图片路径
int main(int argc, char **argv)
{
	int start, end;
	int opration = 0;
	std::vector<tracedata> t_rect;
	char file[] = "t1.avi";
	std::cout << "选择你的操作:\n1.背景提取. 2.车辆跟踪" << std::endl;
	while (opration != 1 && opration != 2)
	{
		std::cin >> opration;
	}
	switch (opration)
	{
	case 1:
		std::cout << "输入开始帧和结束帧:" << std::endl;
		std::cin >> start >> end;
		//ExtractBackground(argv[1], start, end);
		ExtractBackground(file, start, end);
		break;
	case 2:

		char bg[50];
		sprintf(bg, "./%d_to_%d%s", start, end, ".png");
		MobileDectect(argv[1], argv[2], t_rect);
		break;
	default:break;
	}
	outPut(t_rect);
	cout << t_rect.size() << endl;
}
