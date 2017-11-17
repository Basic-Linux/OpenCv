#include "SVM_Bc.h"

//ȫ�ֱ���
Size sampleSize(160, 160);//�����Ĵ�С
int train_samples = 10;
int classes = 4;
Mat trainData;
Mat trainClasses;

//����ȫ�ֺ���
Mat readImageSaveContour(Mat src);
void getData();

int main()
{
	SVM_Bc svm;
	svm.setParam();
	svm.getData();
	svm.train();
	Mat src = imread("test.png",0);
	svm.predicte(src);

	waitKey(0);
}

void getData()
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
			resize(image, imageNewSize, sampleSize); //��ͼƬ����ѹ��������imageNewSize
			image.release();
			image = imageNewSize.reshape(1, 1);//�任ͼ�������� 160 * 160 �� 25600 * 1
			image.convertTo(trainData(Range(i*train_samples + j, i*train_samples + j + 1), Range(0, trainData.cols)), CV_32F);//����������������
			trainClasses.at<int>(i*train_samples + j, 0) = i;//�������ݵ����ͣ�������������ͼ�Σ�������int
		}
	}
}

Mat readImageSaveContour(Mat src)
{
	Mat imageWhite;
	double tmp = threshold(src, imageWhite, 100, 255, 8);
	imageWhite = 255 - imageWhite;//��ת������ֵ�����ͼƬ
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(imageWhite, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);//�ҳ����е�������Ϣ
	//�������
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