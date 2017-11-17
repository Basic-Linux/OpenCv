// 3- RoundRecognition.cpp : Defines the entry point for the console application.
//

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
	//��������ͷ
	VideoCapture capture;
	capture.open(0);

	Mat edges;  //����ת���ĻҶ�ͼ
	if (!capture.isOpened())
		return -1;
	namedWindow("��Ч��ͼ��", CV_WINDOW_NORMAL);
	while (1)
	{
		Mat frame;
		capture >> frame;
		if (!frame.data)
			return -1;
		cvtColor(frame, edges, CV_BGR2GRAY);
		//��˹�˲�
		GaussianBlur(edges, edges, Size(7, 7), 2, 2);
		vector<Vec3f> circles;
		//����Բ
		HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1.5, 10, 150, 100, 0, 0);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			//����Բ��  
			circle(frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			//����Բ����  
			circle(frame, center, radius, Scalar(155, 50, 255), 3, 8, 0);
		}


		imshow("��Ч��ͼ��", frame);


		waitKey(30);

	}

	return 0;
}