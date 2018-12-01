#ifndef __SOCKETMATTRANSMISSIONCLIENT_H__  
#define __SOCKETMATTRANSMISSIONCLIENT_H__  

#ifdef _WIN32                           //windows下的头文件
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <opencv2\core.hpp>
#include <opencv2\opencv.hpp>
#include<WinSock2.h>
#pragma comment(lib,"WS2_32.lib")
#else                                 //linux下的头文件
#include "opencv2/opencv.hpp"  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#endif
using namespace cv;  
  
//待传输图像默认大小为 640*480，可修改  
#define IMG_WIDTH 640   // 需传输图像的宽  
#define IMG_HEIGHT 480  // 需传输图像的高  
#define PACKAGE_NUM 2  
//默认格式为CV_8UC3  
#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/PACKAGE_NUM  //定义发送包的大小
  
struct sentbuf      //发送的结构体 buf是像素  flag判断是第几块的像素
{  
    char buf[BUFFER_SIZE];  
    int flag;  
};  
  
typedef struct SDATA {
	int s_sock;
	Rect selection;
	int trackObject;

};
class SocketMatTransmissionClient  
{  
public:  
    SocketMatTransmissionClient(void);  
    ~SocketMatTransmissionClient(void);  
protected:
	struct sockaddr_in m_servaddr;   //创建结构体记录IP 端口号 和 通信方式
	int m_sockClient;     //socket数据
	SDATA m_data;

private:  
  //  int m_sockClient;     //socket数据
    struct sentbuf data;  //发送的数据

public:  
  
    // 打开socket连接  
    // params : IP      服务器的ip地址  
    //          PORT    传输端口  
    // return : -1      连接失败  
    //          1       连接成功  
    int Createsocket(int PORT);  
  
  
    // 传输图像  
    // params : image 待传输图像  
    // return : -1      传输失败  
    //          1       传输成功  
    int transmit(cv::Mat image, const char* IP,int PORT);
  
	int recvmessage();
    // 断开socket连接  
    void socketDisconnect(void);  
};  
  
#endif  