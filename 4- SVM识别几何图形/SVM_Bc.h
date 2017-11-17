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
	Size sampleSize;//样本的大小
	int train_samples;
	int classes;
	Mat trainData;
	Mat trainClasses;
	int thresholdValue;

	Ptr<SVM> svm;

public:
	SVM_Bc();
	virtual ~SVM_Bc();

	//获取数据源（第二步）
	void getData();
	//设置SVM参数（第一步）
	void setParam();
	//训练数据（第三步）
	void train();
	//进行结果预测（第四步） 传入参数必须为灰值图像
	void predicte(Mat &src);
	//获取某原始训练图像中的轮廓
	Mat readImageSaveContour(Mat& src);
};

