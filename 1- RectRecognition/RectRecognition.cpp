// 1- RectRecognition.cpp : Defines the entry point for the console application.
//

#include "DealSquares.h"

//����ͼ�����ŷ�Ϊ�������� 1 �˲���ȡ�������ȣ� 2 ͼ��任����� 3 ͼ��ԭ�����
int main(int argc, const char* argv[])
{
	DealSquares deal("/home/pi/Desktop/Linux OpenCv/1- RectRecognition/test.png");
	deal.prepare();
	deal.showFinal();

	cout << "�ȴ�����" << endl;
	waitKey(0);
	
	return 0;
}

