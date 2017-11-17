#pragma once
#include <opencv2/ml.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;

using namespace cv::ml;

class SVM_Bc
{
private:
	Size sampleSize;//�����Ĵ�С
	int train_samples;
	int classes;
	Mat trainData;
	Mat trainClasses;
	int thresholdValue;

	Ptr<SVM> svm;

public:
	SVM_Bc();
	virtual ~SVM_Bc();

	//��ȡ����Դ���ڶ�����
	void getData();
	//����SVM��������һ����
	void setParam();
	//ѵ�����ݣ���������
	void train();
	//���н��Ԥ�⣨���Ĳ��� �����������Ϊ��ֵͼ��
	void predicte(Mat &src);
	//��ȡĳԭʼѵ��ͼ���е�����
	Mat readImageSaveContour(Mat& src);
};

