#include "DealSquares.h"


DealSquares::DealSquares(const char * argv)
{
	Mat img = imread(argv, 1);//Mat�ú�������Ҫ�����ͼ����
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
	//֤����������������ڻ���ʽ
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void DealSquares::drawSquaresInImg(Mat& showImg)
{
	//������ֻ��Ҫ����ֱ�߼���
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
		//������ĩ����
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
	//imshow("ԭͼͼ��", img);
	//1 �˲�     
	// ��������
	pyrDown(timg, timg);//ƽ���˲�
	pyrUp(timg, timg);
	//imshow("ԭͼ���봦������ͼ��",timg);
	//2 ͼ��任���˴���Ҫ���е�ͨ����������ȡ��Ե��Ϣ��
	Mat mv[3];
	split(timg, mv);
	for (int i = 0; i < timg.channels(); i++)
	{	
		//���ݻҶ�ֵ���б�Ե��� NֵԽ��Խ��ϸ��������ʱ��Խ��
		for (int l = 0; l < N; l++)
		{
			Mat gray = mv[i];
			if (l == 0)
			{
				Canny(gray, gray, cannyLowLevel, cannyHighLevel);//�񻯣������˳���Ե
				dilate(gray, gray, Mat());//����
			}
			else
			{
				threshold(gray, gray, l * 255 / N, 255, CV_THRESH_BINARY);//�񻯣������˳���Ե
				dilate(gray, gray, Mat());
			}

			//imshow("canny����", gray);
			vector< vector<Point> > contours;
			vector<Vec4i> hierarchy;
			// �ҵ������������Ҵ洢��������
			findContours(gray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

			// ����������Ϣ�����������ж�
			for (int i = 0; i < contours.size(); i++)
			{
				vector<Point> result;
				//����ν���
				approxPolyDP(contours[i], result, dsistanceMax, true);
				//����������Ϣ�ж��Ƿ��ǳ����Σ����Ҵ�С�� areaMin areaMax֮��
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
					//�����s������ֵ 83�ȵ�����ֵ��0.1
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
