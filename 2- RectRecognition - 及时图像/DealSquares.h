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
*	���ߣ� Bc 
*	��ϵ��ʽ��1808150160@qq.com
*	ʱ�䣺2017/10/01
*	���ܣ�
*	���ĵ��ṩһ������OpenCv������ͼƬ�о��εĹ���
*	ʹ�÷���Ϊ��
*	1 ������DealSquares�����
*	2 ����prepare()����������ɸѡ����
*	3 ͨ��showFinal()���������н����ʾ
*	PS�����ĵ����ṩ����ʹ�õ���OpenCv3.1�汾���﷨֧��C++98�Լ����ϰ汾������
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
	//ɸѡ���δ���
	void prepare(){
		findSquares();
		drawSquaresInImg(img);
	}
	//��ʾ��������Դͼ
	void showFinal(){
		imshow("Final Img", img);
	}
	//���û��߲��������������Ϣ�뿴������������ע
	void setLineColor(int r,int g,int b)
	{
		this->color = Scalar(b, g, r);
	}
	//���þ���ɸѡ���������������Ϣ�뿴������������ע
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
	//angle�����������أ���������֮���ҵ��Ƕȵ�����ֵ��
	double angle(Point& pt1, Point& pt2, Point& pt0);
	//�������Ϊ��ʾ������ͼƬ����ʾ�����ķ���
	void drawSquaresInImg(Mat& showImg);
	//���ĺ���������ɸѡ����
	vector< vector<Point> > findSquares();
private:
	Mat img;//ԴͼƬ
	Mat timg;//��ʱͼƬ��Ϊ�˱��������ԭͼƬ��
	vector< vector<Point> > squares;
	//��ȷ�Ȳ���
	//����ν���ʱ������
	int dsistanceMax;
	//�ܹ������������/��С���������
	int areaMax;
	int areaMin;
	//�ж�����ֵ����Сֵ
	float cosValueMin;
	//��ֵ�ָϸ�̶�,��ֵԽ��ͼ�����Խ��ȷ�����㷨ʱ��Խ������СΪ1
	int N;
	//Canny���������ֵ����
	int cannyHighLevel;
	int cannyLowLevel;

	//��ͼ���
	//�ߵ���ɫ
	Scalar color;
	//�ߵĿ��
	int lineWidth;
	//�ߵ�����
	int lineType;
};

#endif

