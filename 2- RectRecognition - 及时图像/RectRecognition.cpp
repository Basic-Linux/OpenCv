// 1- RectRecognition.cpp : Defines the entry point for the console application.
//

#include "DealSquares.h"

//����ͼ�����ŷ�Ϊ�������� 1 �˲���ȡ�������ȣ� 2 ͼ��任����� 3 ͼ��ԭ�����
int main(int argc, const char* argv[])
{
	//��������ͷ
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
	

	cout << "�ȴ�����" << endl;
	waitKey(0);
	
	return 0;
}

