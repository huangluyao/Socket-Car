#include "SocketMatTransmissionClient.h"  
  
SocketMatTransmissionClient::SocketMatTransmissionClient(void)  
{  
}  
  
  
SocketMatTransmissionClient::~SocketMatTransmissionClient(void)  
{  
}  
  
  
int SocketMatTransmissionClient::Createsocket(int PORT)  
{  
#ifdef _WIN32    //windows下socket的初始化
	WORD wVersionRequested;    //获取版本号
	WSADATA wsaData;         
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData); //创建初始化句柄

	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||          //创建失败则退出
		HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
#endif

    if ((m_sockClient = socket(AF_INET, SOCK_DGRAM, 0)) < 0)    //创建socket句柄，采用TCP协议
 //   if ((m_sockClient = socket(AF_INET, SOCK_STREAM, 0)) < 0)    //创建socket句柄，采用TCP协议
    {  
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;  
    }  
  
//    memset(&m_servaddr, 0, sizeof(m_servaddr));  //初始化结构体
	m_servaddr.sin_family = AF_INET;           //设置通信方式
	m_servaddr.sin_port = htons(PORT);         //设置端口号

	bind(m_sockClient, (sockaddr*)&m_servaddr, sizeof(m_servaddr));

	memset(&m_servaddr, 0, sizeof(m_servaddr));  //初始化结构体
	m_servaddr.sin_family = AF_INET;           //设置通信方式
	m_servaddr.sin_port = htons(0x1234);         //设置端口号
	m_servaddr.sin_addr.s_addr = inet_addr("192.168.43.17");
//	connect(m_sockClient, (sockaddr*)&m_servaddr, sizeof(m_servaddr));
}  
  
  
void SocketMatTransmissionClient::socketDisconnect(void)  
{  
#ifdef _WIN32     //windows下关闭socket 和linux下不一样
    closesocket(m_sockClient);  
#else
	close(m_sockClient);
#endif
}  
  
int SocketMatTransmissionClient::transmit(cv::Mat image, const char* IP, int PORT)
{  
    if (image.empty())    //如果照片为空则退出
    {  
        printf("empty image\n\n");  
        return -1;  
    }  
	std::vector<uchar> data_encode;
	std::vector<int> quality;

	quality.push_back(CV_IMWRITE_JPEG_QUALITY);
	quality.push_back(50);
	imencode(".jpg", image, data_encode, quality);
	char encodeImg[65535];

	//char *pchar = encodeImg;
	//std::vector<uchar>::iterator it=data_encode.begin;
	//C++ 11新语法  不适合G++老版本
	/*for (auto val : data_encode)
	{
		*pchar = val;
		pchar += 1;
	}*/

	int nSize = data_encode.size();
	for (int i = 0; i < nSize; i++)
	{
		encodeImg[i] = data_encode[i];
	}
	m_servaddr.sin_addr.s_addr = inet_addr(IP);
	m_servaddr.sin_port = htons(PORT);
	sendto(m_sockClient, encodeImg, nSize,0,(sockaddr*)&m_servaddr,sizeof(m_servaddr));
	//send(m_sockClient, encodeImg, nSize,0);
}

int SocketMatTransmissionClient::recvmessage()
{
	cv::Rect selectRect;
	int n = recv(m_sockClient, (char*)&selectRect, sizeof(selectRect), 0);
	if (n < 0)
		return -1;
	m_data.selection = selectRect;
	m_data.trackObject = -1;
	return n;
}


//
//if (image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC3)  //摄像头读取的照片与设定高度宽度类型符合则打印错误
//{
//	printf("the image must satisfy : cols == IMG_WIDTH（%d）  rows == IMG_HEIGHT（%d） type == CV_8UC3\n\n", IMG_WIDTH, IMG_HEIGHT);
//	return -1;
//}
//
//for (int k = 0; k < PACKAGE_NUM; k++)   //分两个包传输
//{
//	int num1 = IMG_HEIGHT / PACKAGE_NUM * k;              //取一半高的图片像素
//	for (int i = 0; i < IMG_HEIGHT / PACKAGE_NUM; i++)
//	{
//		int num2 = i * IMG_WIDTH * 3;                    //mun2 记录每一行的个数以data.buf累加
//		uchar* ucdata = image.ptr<uchar>(i + num1);      //读取每一行的首地址
//		for (int j = 0; j < IMG_WIDTH * 3; j++)
//		{
//			data.buf[num2 + j] = ucdata[j];              //记录图片中一半的数据
//		}
//	}
//
//	if (k == PACKAGE_NUM - 1)		//判断数据是上半行还是下半行
//		data.flag = 2;
//	else
//		data.flag = 1;
//
//	if (send(sockClient, (char *)(&data), sizeof(data), 0) < 0)  //发送一半的图片
//	{
//		printf("send image error: %s(errno: %d)\n", strerror(errno), errno);
//		return -1;
//	}
//}