#include "CarControl.h"


double CCarControl::GetDistance(int Tr, int Ec)
{
#ifndef _WIN32

	struct timeval tv1;	//启动linux下自带的计时器
	struct timeval tv2;
	long start, stop;	//定义开始和停止时间
	float dis;			//距离 这个是返回值
	digitalWrite(Tr, LOW);  //超声波发射端拉准备发出信号
	delayMicroseconds(2);	//延时2us
							//std::cout << "Start Send ultrasonic" << std::endl;
	digitalWrite(Tr, HIGH);//超声波发射端拉高，发射信号
	delayMicroseconds(10);	  //发出超声波脉冲
	digitalWrite(Tr, LOW);	//超声波发射端拉低 准备下一次的发送
//	std::cout << "Sending ultrasonic" << std::endl;
	int count = 0;
	while (!(digitalRead(Ec) == 1)&&count<30000)	//接收端一直位高
	{
		count++;
	}
	if (count > 30000)
	{
		return 0; //接收端没有信号就退出
	}
	gettimeofday(&tv1, NULL);		   //获取当前时间
//	std::cout << "Start Recive ultrasonic" << std::endl;
	while (!(digitalRead(Ec) == 0));	//接收到返回信号后，超声波接收端口为低电平
	gettimeofday(&tv2, NULL);		   //获取当前时间
	//std::cout << "Recived ultrasonic" << std::endl;
	start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //微秒级的时间
	stop = tv2.tv_sec * 1000000 + tv2.tv_usec;

	dis = (float)(stop - start) / 1000000 * 34000 / 2;  //求出距离

	return dis;

#endif // !_WIN32
	return 0;
}

CCarControl::CCarControl()
{
#ifndef _WIN32
	wiringPiSetup();//树莓派端口初始化
	pinMode(1, OUTPUT);	//IN1
	pinMode(4, OUTPUT);	//IN2
	pinMode(5, OUTPUT);	//IN3
	pinMode(6, OUTPUT);	//IN4
						//超声波管脚初始化
	pinMode(Echo, INPUT);
	pinMode(Trig, OUTPUT);
	pinMode(Echo1, INPUT);
	pinMode(Trig1, OUTPUT);

	//小车PWM初始化
	softPwmCreate(0, 1, 500);
	softPwmCreate(2, 1, 500);

#endif // _WIN32

}

CCarControl::CCarControl(int * px, int * py)
{
	m_pX = px;
	m_pY = py;
}


CCarControl::~CCarControl()
{
#ifndef	_WIN32
	brake();
#endif
}

void CCarControl::CarRun()
{

#ifndef _WIN32
	wiringPiSetup();//树莓派端口初始化
	pinMode(1, OUTPUT);	//IN1
	pinMode(4, OUTPUT);	//IN2
	pinMode(5, OUTPUT);	//IN3
	pinMode(6, OUTPUT);	//IN4
						//超声波管脚初始化
	pinMode(Echo, INPUT);
	pinMode(Trig, OUTPUT);
	pinMode(Echo1, INPUT);
	pinMode(Trig1, OUTPUT);

	//小车PWM初始化
	softPwmCreate(0, 1, 500);
	softPwmCreate(2, 1, 500);

	while (1)
	{
		m_disR = GetDistance(Trig, Echo);	//左侧距离计算
		m_disL = GetDistance(Trig1, Echo1);//右侧距离计算
		std::cout << "nDistanceR:" << m_disR << std::endl;
		std::cout << "nDistanceL:" << m_disL << std::endl;
		std::cout << "Point of X:" << *m_pX <<std::endl;

		if (*m_pX > 410&&*m_pX<620)
		{
			left();
			std::cout << "turn left" << std::endl;
		}
		if (*m_pX < 230 &&*m_pX>20)
		{
			right();
			std::cout << "turn right" << std::endl;
		}
		if (*m_pX >= 230 && *m_pX <= 410)
		{
			if ((m_disL < 50 && m_disL >30) || (m_disR > 30 && m_disR < 50))
			{
				brake();
			//	std::cout << "brake" << std::endl;
			}
			else if (m_disL < 30 || m_disR < 30)
			{
				back();
			//	std::cout << "back" << std::endl;
			}
			else
			{
				Run();
			//	std::cout << "Run" << std::endl;
			}
		}
	}
#else
	while (1)
	{
		std::cout << "Point of X:" << *m_pX << std::endl;
	}
#endif // !_WIN32

}
