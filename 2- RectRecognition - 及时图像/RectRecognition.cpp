// 1- RectRecognition.cpp : Defines the entry point for the console application.
//

#include "DealSquares.h"

//进行图像处理大概分为三个步骤 1 滤波（取出噪音等） 2 图像变换与检索 3 图像复原与呈现
int main(int argc, const char* argv[])
{
	//开起摄像头
	VideoCapture capture;
	capture.open(0);
	namedWindow("Final Img", CV_WINDOW_NORMAL);
	while (1)
	{
		Mat frame;
		capture >> frame;
		if (!frame.data)
			return -1;
		DealSquares deal(frame);
		deal.prepare();
		deal.showFinal();

		waitKey(30);
		
	}
	

	cout << "等待按键" << endl;
	waitKey(0);
	
	return 0;
}

