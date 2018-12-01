#include "DetectImage.h"
using namespace std;
using namespace cv;



int CDetectImage::GetCircle(cv::Mat srcImg, cv::Point & center, int & radius)
{
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);//转化边缘检测后的图为灰度图
	GaussianBlur(srcImg, srcImg, Size(9, 9), 2, 2);

	std::vector<Vec3f> circles;
	HoughCircles(srcImg, circles, CV_HOUGH_GRADIENT, 1.5, 10, 200, 75, 0, 0);

	for (size_t i = 0; i < circles.size(); i++)
	{
		Point point(cvRound(circles[i][0]), cvRound(circles[i][1]));
		center = point;
		radius = cvRound(circles[i][2]);
		return 1;
	}
	return 0;
}

CDetectImage::CDetectImage()
{
	histimg = Mat::zeros(200, 320, CV_8UC3);
	m_hranges[0] = 0;
	m_hranges[1] = 180;
	m_vmin = 10;
	m_vmax = 256;
	smin = 30;
	m_data.trackObject =0 ;
	hsize = 16;
	m_phranges = m_hranges;
	capture.open(0);
//	sample = cv::imread("sample.jpg");
	Createsocket(0x4321);
	m_data.s_sock = m_sockClient;
}


CDetectImage::~CDetectImage()
{
}

void CDetectImage::GetPoint()
{
	capture >> srcImage;
#ifdef _WIN32

	clock_t duration = clock() - nLastFRTime;// 设置时间判定，防止长时间占用CPU
	if (duration < 30)
	{
		Sleep(1);
		return;
	}
	nLastFRTime = clock();
#else
	usleep(30);
#endif // _WIN32


	if (srcImage.empty())
	{
		std::cout << "Can not open camrea." << endl;
		return;
	}
	if(m_data.trackObject)
	{ 
		//std::cout << "hello" << std::endl;
		cvtColor(srcImage, hsv, COLOR_BGR2HSV);  //将RGB转化成HSV
		inRange(hsv, Scalar(0, smin, MIN(m_vmin, m_vmax)),
			Scalar(180, 256, MAX(m_vmin, m_vmax)), mask);
		int ch[] = { 0, 0 };
		hue.create(hsv.size(), hsv.depth());    //建立一个图像
												//mixChannels主要就是把输入的矩阵（或矩阵数组）的某些通道拆分复制给对应的输出矩阵（或矩阵数组）的某些通道中，其中的对应关系就由fromTo参数制定.
		mixChannels(&hsv, 1, &hue, 1, ch, 1);
		if (m_data.trackObject < 0)
		{
			
			cvtColor(srcImage(m_data.selection), SampleHsv, COLOR_BGR2HSV);//样本
			inRange(SampleHsv, Scalar(0, smin, MIN(m_vmin, m_vmax)),
				Scalar(180, 256, MAX(m_vmin, m_vmax)), SampleMask);
			Mat sampleHue(SampleHsv.size(), SampleHsv.depth());
			mixChannels(&SampleHsv, 1, &sampleHue, 1, ch, 1);
			calcHist(&sampleHue, 1, 0, SampleMask, hist, 1, &hsize, &m_phranges);

			normalize(hist, hist, 0, 255, NORM_MINMAX);   //矩阵归一化
														  //此句代码的OpenCV2版为：
														  //normalize(hist, hist, 0, 255, CV_MINMAX);
			trackWindow = m_data.selection;
			m_data.trackObject = 1;
			histimg = Scalar::all(0);
			int binW = histimg.cols / hsize;
			Mat buf(1, hsize, CV_8UC3);
			for (int i = 0; i < hsize; i++)
				buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180. / hsize), 255, 255);

			cvtColor(buf, buf, COLOR_HSV2BGR);

			for (int i = 0; i < hsize; i++)
			{
				int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows / 255);
				rectangle(histimg, Point(i*binW, histimg.rows),
					Point((i + 1)*binW, histimg.rows - val),
					Scalar(buf.at<Vec3b>(i)), -1, 8);
			}
			//m_data.trackObject = -1;
		}
		cv::calcBackProject(&hue, 1, 0, hist, backproj, &m_phranges);
		backproj &= mask;
		RotatedRect trackBox = CamShift(backproj, trackWindow,


			TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));


		if (trackWindow.area() <= 1)
		{
			int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5) / 6;
			trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
				trackWindow.x + r, trackWindow.y + r) &
				Rect(0, 0, cols, rows);
		}

		if (trackBox.size.width <= 10 | trackBox.size.height <= 10)
		{
			m_dis.x = trackBox.center.x;
			m_dis.y = trackBox.center.y;
			return;
		}
		cv::ellipse(srcImage, trackBox, Scalar(0, 0, 255), 3, LINE_AA);
		m_dis.x = trackBox.center.x;
		m_dis.y = trackBox.center.y;
	}
#ifdef _WIN32
	transmit(srcImage, "127.0.0.1", 0x1234);

#else
	//transmit(srcImage, "192.168.1.101", 0x1234);
	transmit(srcImage, "192.168.43.17", 0x1234);

#endif // _WIN32


}
