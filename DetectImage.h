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
	int hsize;  //存放每个维度的直方图尺寸
	float m_hranges[2];
	const float* m_phranges; //每个维度的取值范围
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

