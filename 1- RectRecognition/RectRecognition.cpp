// 1- RectRecognition.cpp : Defines the entry point for the console application.
//

#include "DealSquares.h"

//进行图像处理大概分为三个步骤 1 滤波（取出噪音等） 2 图像变换与检索 3 图像复原与呈现
int main(int argc, const char* argv[])
{
	DealSquares deal("/home/pi/Desktop/Linux OpenCv/1- RectRecognition/test.png");
	deal.prepare();
	deal.showFinal();

	cout << "等待按键" << endl;
	waitKey(0);
	
	return 0;
}

