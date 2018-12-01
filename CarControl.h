#pragma once
#ifdef _WIN32
#include <process.h>
#else
#include <pthread.h>   //linux系统下的多线程头文件
#endif
#ifndef _WIN32
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
//超声波管脚定义
#define Trig	26
#define Echo	27
#define Trig1	24
#define Echo1	25
#endif // _WIN32
#include <iostream>

class CCarControl
{


	double m_disR;
	double m_disL;
	int* m_pX;
	int* m_pY;
#ifndef _WIN32


	void Run()
	{
		softPwmWrite(0, 250);  //全速前进 
		softPwmWrite(2, 250);
		digitalWrite(1, HIGH);
		digitalWrite(4, LOW);
		digitalWrite(5, HIGH);
		digitalWrite(6, LOW);
	}
	void brake()
	{
		digitalWrite(1, LOW);
		digitalWrite(4, LOW);
		digitalWrite(5, LOW);
		digitalWrite(6, LOW);
	}

	void left()
	{
		softPwmWrite(0, 200);  //全速前进 
		softPwmWrite(2, 200);
		digitalWrite(4, HIGH);
		digitalWrite(1, LOW);
		digitalWrite(5, HIGH);
		digitalWrite(6, LOW);
	}

	void right()
	{
		softPwmWrite(0, 200);  //全速前进 
		softPwmWrite(2, 200);
		digitalWrite(4, LOW);
		digitalWrite(1, HIGH);
		digitalWrite(5, LOW);
		digitalWrite(6, HIGH);
	}

	void back()
	{
		softPwmWrite(0, 200);  //全速前进 
		softPwmWrite(2, 200);
		digitalWrite(4, HIGH);
		digitalWrite(1, LOW);
		digitalWrite(6, HIGH);
		digitalWrite(5, LOW);
	}
#endif // !_WIN32
	double GetDistance(int Tr, int Ec);
public:
	CCarControl();
	CCarControl(int * px,int*py);

	~CCarControl();
	void CarRun();
};

