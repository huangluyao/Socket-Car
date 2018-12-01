#pragma once
#include "SocketMatTransmissionClient.h"
#include <opencv2/opencv.hpp>
struct pPointData
{
	int x;
	int y;
};

class CDetectImage:public SocketMatTransmissionClient
{
private:
	int GetCircle(cv::Mat srcImg, cv::Point& center, int &radius);
	cv::Mat hsv, hue, mask, hist, histimg, backproj;
//	cv::Mat sample;
	cv::Mat SampleHsv, SampleMask;
	cv::Mat srcImage;
	cv::VideoCapture capture;
	cv::Point center;
	cv::Rect trackWindow;
	int hsize;  //���ÿ��ά�ȵ�ֱ��ͼ�ߴ�
	float m_hranges[2];
	const float* m_phranges; //ÿ��ά�ȵ�ȡֵ��Χ
	int m_vmin;
	int m_vmax;
	int smin;
	int nLastFRTime;
public:
	CDetectImage();
	~CDetectImage();
	pPointData m_dis;
	void GetPoint();
};

