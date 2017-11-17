#pragma once
#ifndef DealSquares_H_
#define DealSquares_H_
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <math.h>
#include <string.h>
#include <iostream>
/*
*	作者： Bc 
*	联系方式：1808150160@qq.com
*	时间：2017/10/01
*	介绍：
*	本文档提供一个利用OpenCv来检索图片中矩形的功能
*	使用方法为：
*	1 创建出DealSquares类对象
*	2 调用prepare()函数来进行筛选工作
*	3 通过showFinal()函数来进行结果显示
*	PS：本文档所提供代码使用的是OpenCv3.1版本，语法支持C++98以及以上版本编译器
*/

using namespace std;
using namespace cv;
struct SquaresParams{
	int dsistanceMax;
	int areaMax;
	int areaMin;
	float cosValueMin;
	int N;
	int cannyHighLevel;
	int cannyLowLevel;
};

class DealSquares
{
public:
	DealSquares(const char * argv);
	DealSquares(Mat& src);
	virtual ~DealSquares();
	//筛选矩形处理
	void prepare(){
		findSquares();
		drawSquaresInImg(img);
	}
	//显示处理结果到源图
	void showFinal(){
		imshow("Final Img", img);
	}
	//设置画线参数，具体参数信息请看类属性声明备注
	void setLineColor(int r,int g,int b)
	{
		this->color = Scalar(b, g, r);
	}
	//设置矩形筛选参数，具体参数信息请看类属性声明备注
	void setParams(struct SquaresParams &params)
	{
		int *p = (int *)&params;
		for (int i = 0; i < sizeof(struct SquaresParams) / sizeof(int); i++)
		{
			if (p[i] == -1)
				continue;
			switch (i)
			{
				case 0:dsistanceMax = p[i]; break;
				case 1:areaMax = p[i]; break;
				case 2:areaMin = p[i]; break;
				case 3:cosValueMin = p[i]; break;
				case 4:if (p[i] <= 0) break;
					   else N = p[i]; 
						break;
				case 5:cannyHighLevel = p[i]; break;
				case 6:cannyLowLevel = p[i]; break;
				default:break;
			}		
		}
	}
private:
	//angle函数用来返回（两个向量之间找到角度的余弦值）
	double angle(Point& pt1, Point& pt2, Point& pt0);
	//传入参数为显示在哪张图片上显示画出的方框
	void drawSquaresInImg(Mat& showImg);
	//核心函数，用来筛选矩形
	vector< vector<Point> > findSquares();
private:
	Mat img;//源图片
	Mat timg;//临时图片（为了避免误操作原图片）
	vector< vector<Point> > squares;
	//精确度参数
	//多边形近似时最大点间距
	int dsistanceMax;
	//能够检测出来的最大/最小正方形面积
	int areaMax;
	int areaMin;
	//判断余弦值的最小值
	float cosValueMin;
	//阈值分割精细程度,该值越大图像分析越精确，但算法时间越长，最小为1
	int N;
	//Canny检测上限阈值程序
	int cannyHighLevel;
	int cannyLowLevel;

	//绘图相关
	//线的颜色
	Scalar color;
	//线的宽度
	int lineWidth;
	//线的种类
	int lineType;
};

#endif

