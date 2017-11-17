#include "SVM_Bc.h"

SVM_Bc::SVM_Bc()
{
	// Set up SVM's parameters
	svm = SVM::create();//建立一个空的svm文件
	//全局变量
	sampleSize = Size (160, 160);//样本的大小
	train_samples = 10;
	classes = 4;
	thresholdValue = 100;
}


SVM_Bc::~SVM_Bc()
{
}

void SVM_Bc::setParam()
{
	//设置SVM参数
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.001));//SVM的迭代训练过程的中止条件
	svm->setC(1);
	svm->setCoef0(0);
	svm->setDegree(0);
	svm->setGamma(1);
	svm->setP(0);
	svm->setNu(0);
}

void SVM_Bc::train()
{
	// Train the SVM
	svm->StatModel::train(trainData, ROW_SAMPLE, trainClasses);
}

void SVM_Bc::getData()
{
	trainData.create(train_samples*classes, sampleSize.width*sampleSize.height, CV_32F);
	trainClasses.create(train_samples*classes, 1, CV_32SC1);
	Mat src_image;
	char file[255];
	int i, j;
	for (i = 0; i<classes; i++)
	{
		for (j = 0; j< train_samples; j++)
		{
			sprintf(file, "samples/s%d/%d.png", i, j);
			src_image = imread(file, 0);
			if (src_image.empty())
			{
				printf("Error: Cant load image %s\n", file);
				//exit(-1);
			}
			Mat image = readImageSaveContour(src_image);
			Mat imageNewSize;
			resize(image, imageNewSize, sampleSize); //将图片进行压缩并放入imageNewSize
			image.release();
			image = imageNewSize.reshape(1, 1);//变换图像行列由 160 * 160 到 25600 * 1
			image.convertTo(trainData(Range(i*train_samples + j, i*train_samples + j + 1), Range(0, trainData.cols)), CV_32F);//存入数据轮廓集合
			trainClasses.at<int>(i*train_samples + j, 0) = i;//存入数据的类型（即几种轮廓的图形）必须是int
		}
	}
}

void SVM_Bc::predicte(Mat &img)
{
	Mat src;
	cvtColor(img,src,CV_RGB2GRAY);
	//得到图像进行预测
	//Mat src = imread("test.png", 0);
	Mat show;
	cvtColor(src, show, 8);//8 表示灰度图到彩色图
	Mat imageWhite;
	threshold(src, imageWhite, thresholdValue, 255, 8);
	imageWhite = 255 - imageWhite;
	//通过轮廓信息进行相关算法
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(imageWhite, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	for (int index = contours.size() - 1; index >= 0; index--)
	{
		Rect rectangleTem = boundingRect(contours[index]);
		double tmparea = fabs( contourArea(contours[index]) );
		
		if(tmparea < 100)
		{
		
			continue;
		}
		printf("area = %f ",tmparea);
		Mat image;
		image = Mat::zeros(src.size(), CV_8UC1);
		drawContours(image, contours, index, Scalar(255), 2, 8, hierarchy);
		Mat tem = image(rectangleTem);
		Mat imageNewSize;
		resize(tem, imageNewSize, sampleSize, CV_INTER_LINEAR);
		image.release();
		image = imageNewSize.reshape(1, 1);
		image.convertTo(image, CV_32F);
		//进行SVM分析
		float response = svm->predict(image);
		printf("response = %f\n",response);
		float calibration = 0.000001;
		//根据分析得到的值进行判断数据类型
		if (response >= 0 - calibration && response <= 0 + calibration)
		{
			cout << "    circle" << endl;
			string str = "circle";
			putText(show, str, Point(rectangleTem.x + rectangleTem.width / 2, rectangleTem.y + rectangleTem.height / 2),
				FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1, 8);
		}
		else if (response >= 1 - calibration && response <= 1 + calibration)
		{
			cout << "    rectangle" << endl;
			string str = "rectangle";
			putText(show, str, Point(rectangleTem.x + rectangleTem.width / 2, rectangleTem.y + rectangleTem.height / 2),
				FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1, 8);
		}
		else if (response >= 2 - calibration && response <= 2 + calibration)
		{
			cout << "    triangle" << endl;
			string str = "triangle";
			putText(show, str, Point(rectangleTem.x + rectangleTem.width / 2, rectangleTem.y + rectangleTem.height / 2),
				FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1, 8);
		}
		else if (response >= 3 - calibration && response <= 3 + calibration)
		{
			cout << "    cross" << endl;
			string str = "cross";
			putText(show, str, Point(rectangleTem.x + rectangleTem.width / 2, rectangleTem.y + rectangleTem.height / 2),
				FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1, 8);
		}
	}
	imshow("result", show);
	//imwrite("result.png", show);
}

Mat SVM_Bc::readImageSaveContour(Mat& src)
{
	Mat imageWhite;
	double tmp = threshold(src, imageWhite, thresholdValue, 255, 8);
	imageWhite = 255 - imageWhite;//翻转经过阈值处理的图片
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(imageWhite, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);//找出其中的轮廓信息
	//最大轮廓
	double maxarea = 0;
	int maxAreaIdx = 0;
	for (int index = contours.size() - 1; index >= 0; index--)
	{
		double tmparea = fabs(contourArea(contours[index]));		
		if (tmparea>maxarea)
		{
			maxarea = tmparea;
			maxAreaIdx = index;
		}
	}
	Rect rectangleTem = boundingRect(contours[maxAreaIdx]);
	Mat image;
	image = Mat::zeros(src.size(), CV_8UC1);
	drawContours(image, contours, 0, Scalar(255), 2, 8, hierarchy);
	//Rect newRectangleTem(rectangleTem.x - 1, rectangleTem.y - 1, rectangleTem.width + 2, rectangleTem.height+2);
	Mat tem = image(rectangleTem);
	return tem;
}
