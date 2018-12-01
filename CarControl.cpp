#include "CarControl.h"


double CCarControl::GetDistance(int Tr, int Ec)
{
#ifndef _WIN32

	struct timeval tv1;	//����linux���Դ��ļ�ʱ��
	struct timeval tv2;
	long start, stop;	//���忪ʼ��ֹͣʱ��
	float dis;			//���� ����Ƿ���ֵ
	digitalWrite(Tr, LOW);  //�������������׼�������ź�
	delayMicroseconds(2);	//��ʱ2us
							//std::cout << "Start Send ultrasonic" << std::endl;
	digitalWrite(Tr, HIGH);//��������������ߣ������ź�
	delayMicroseconds(10);	  //��������������
	digitalWrite(Tr, LOW);	//��������������� ׼����һ�εķ���
//	std::cout << "Sending ultrasonic" << std::endl;
	int count = 0;
	while (!(digitalRead(Ec) == 1)&&count<30000)	//���ն�һֱλ��
	{
		count++;
	}
	if (count > 30000)
	{
		return 0; //���ն�û���źž��˳�
	}
	gettimeofday(&tv1, NULL);		   //��ȡ��ǰʱ��
//	std::cout << "Start Recive ultrasonic" << std::endl;
	while (!(digitalRead(Ec) == 0));	//���յ������źź󣬳��������ն˿�Ϊ�͵�ƽ
	gettimeofday(&tv2, NULL);		   //��ȡ��ǰʱ��
	//std::cout << "Recived ultrasonic" << std::endl;
	start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //΢�뼶��ʱ��
	stop = tv2.tv_sec * 1000000 + tv2.tv_usec;

	dis = (float)(stop - start) / 1000000 * 34000 / 2;  //�������

	return dis;

#endif // !_WIN32
	return 0;
}

CCarControl::CCarControl()
{
#ifndef _WIN32
	wiringPiSetup();//��ݮ�ɶ˿ڳ�ʼ��
	pinMode(1, OUTPUT);	//IN1
	pinMode(4, OUTPUT);	//IN2
	pinMode(5, OUTPUT);	//IN3
	pinMode(6, OUTPUT);	//IN4
						//�������ܽų�ʼ��
	pinMode(Echo, INPUT);
	pinMode(Trig, OUTPUT);
	pinMode(Echo1, INPUT);
	pinMode(Trig1, OUTPUT);

	//С��PWM��ʼ��
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
	wiringPiSetup();//��ݮ�ɶ˿ڳ�ʼ��
	pinMode(1, OUTPUT);	//IN1
	pinMode(4, OUTPUT);	//IN2
	pinMode(5, OUTPUT);	//IN3
	pinMode(6, OUTPUT);	//IN4
						//�������ܽų�ʼ��
	pinMode(Echo, INPUT);
	pinMode(Trig, OUTPUT);
	pinMode(Echo1, INPUT);
	pinMode(Trig1, OUTPUT);

	//С��PWM��ʼ��
	softPwmCreate(0, 1, 500);
	softPwmCreate(2, 1, 500);

	while (1)
	{
		m_disR = GetDistance(Trig, Echo);	//���������
		m_disL = GetDistance(Trig1, Echo1);//�Ҳ�������
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
