#include "DealSquares.h"


DealSquares::DealSquares(const char * argv)
{
	Mat img = imread(argv, 1);//Mat该函数库主要是针对图像处理
	if (!img.data)
	{
		cout << "cannt not load img" << *argv << endl;
	}

	this->img = img;
	this->img.copyTo(timg);
	this->color = Scalar(0, 0, 255);
	this->lineWidth = 2;
	this->lineType = 8;
	this->N = 5;
	cannyHighLevel = 128;
	cannyLowLevel = 0;
	dsistanceMax = 10;

	areaMax = 100000;
	areaMin = 500;
	cosValueMin = 0.2;
}

DealSquares::DealSquares(Mat& src)
{
	this->img = src;
	this->img.copyTo(timg);
	this->color = Scalar(0, 0, 255);
	this->lineWidth = 2;
	this->lineType = 8;
	this->N = 5;
	cannyHighLevel = 128;
	cannyLowLevel = 0;
	dsistanceMax = 10;

	areaMax = 100000;
	areaMin = 500;
	cosValueMin = 0.2;
}

DealSquares::~DealSquares()
{
}

double DealSquares::angle(Point& pt1, Point& pt2, Point& pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	//证明过程请查阅向量内积公式
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void DealSquares::drawSquaresInImg(Mat& showImg)
{
	//接下来只需要画出直线即可
	for (int i = 0; i < squares.size(); i++)
	{
		vector<Point> & list = squares.at(i);
		for (int j = 1; j < squares.at(i).size(); j++)
		{
			line(showImg,
				list.at(j),
				list.at(j - 1),
				color,
				lineWidth,
				lineType);
		}
		//链接首末两点
		line(img,
			list.at(0),
			list.at(list.size() - 1),
			Scalar(0, 0, 255),
			2,
			8);
	}

}

vector< vector<Point> > DealSquares::findSquares()
{
	//imshow("原图图像", img);
	//1 滤波     
	// 过滤噪音
	pyrDown(timg, timg);//平滑滤波
	pyrUp(timg, timg);
	//imshow("原图降噪处理过后的图像",timg);
	//2 图像变换（此处需要进行单通道处理来获取边缘信息）
	Mat mv[3];
	split(timg, mv);
	for (int i = 0; i < timg.channels(); i++)
	{	
		//根据灰度值进行边缘检测 N值越大越精细，但运算时间越长
		for (int l = 0; l < N; l++)
		{
			Mat gray = mv[i];
			if (l == 0)
			{
				Canny(gray, gray, cannyLowLevel, cannyHighLevel);//锐化，用来滤除边缘
				dilate(gray, gray, Mat());//膨胀
			}
			else
			{
				threshold(gray, gray, l * 255 / N, 255, CV_THRESH_BINARY);//锐化，用来滤除边缘
				dilate(gray, gray, Mat());
			}

			//imshow("canny检测后", gray);
			vector< vector<Point> > contours;
			vector<Vec4i> hierarchy;
			// 找到所有轮廓并且存储在序列中
			findContours(gray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

			// 根据轮廓信息进行正方形判断
			for (int i = 0; i < contours.size(); i++)
			{
				vector<Point> result;
				//多边形近似
				approxPolyDP(contours[i], result, dsistanceMax, true);
				//根据轮廓信息判断是否是长方形，并且大小在 areaMin areaMax之间
				if (result.size() == 4 &&
					fabs(contourArea(result)) > areaMin &&
					fabs(contourArea(result)) < areaMax &&
					isContourConvex(result))
				{
					double s = 0.0;
					for (int k = 0; k < 4; k++)
					{
						double t = 0.0;
						// find minimum angle between joint edges (maximum of cosine)
						if (k >= 2)
						{
							t = fabs(angle(
								result.at(k),
								result.at(k - 2),
								result.at(k - 1)));
							s = s > t ? s : t;
						}
					}
					//cout << "s = " << s << endl;
					//这里的s是余弦值 83度的余弦值是0.1
					if (s < cosValueMin)
					{
						squares.push_back(result);
					}

				}
			}
		}
	}

	return squares;
}
