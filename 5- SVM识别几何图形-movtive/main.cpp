#include "SVM_Bc.h"

int main()
{
	
	SVM_Bc svm;
	svm.setParam();
	svm.getData();
	svm.train();
	namedWindow("result", CV_WINDOW_NORMAL);
	VideoCapture capture;
	capture.open(0);
	while (1)
	{
		Mat frame;
		capture >> frame;
		svm.predicte(frame);
		waitKey(20);
	}
	waitKey(0);

}