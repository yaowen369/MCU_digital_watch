//���ļ���Ҫ�Ǵ������壬���㱨ʱ����������

#include<reg52.h>
#include"head.h"

//-------���������,���ȷ��ʮ��֮һ��-------------------//
bit stopwatch_flag = OFF;    //����Ƿ����ı�־
uint stopwatch_hour = 0;
uint stopwatch_minute = 0;
uint stopwatch_second = 0;
uint stopwatch_msec = 0;   //�ٷ�֮һ��

//���Ľ�λ�����λ����,�ֽ�λ��Сʱ
void stopwatch_carry()
{
    if (stopwatch_second == 60)
	{
	    stopwatch_second = 0;
		stopwatch_minute++;
	}

	if (stopwatch_minute == 60)
	{
	    stopwatch_minute = 0;
		stopwatch_hour++;
	}

	if (stopwatch_hour == 100)
	{
	    stopwatch_hour = 0;
	}
}

//��������������ӱ�ģʽ��Ϊ1ʱ�����ĸ���ȫ�ֱ����ĸ�λ
void stopwatch_reset()
{
    stopwatch_flag = OFF;    
    stopwatch_hour = 0;
    stopwatch_minute = 0;
    stopwatch_second = 0;
    stopwatch_msec = 0;   
}

//--------------�������岿��-------------------------//
bit alarm_flag = OFF;   //�ж������Ƿ����ı�־��0�أ�1��
uchar alarm_hour = 0;
uchar alarm_minute = 0;
uchar ringing_flag = OFF;	 //�ж�����������ı�־

//�ж��Ƿ�����ʱ�䵽��
static uchar adjust_ring()
{
    if ((show_hour == alarm_hour) && (show_minute == alarm_minute))
	{
	    return 1;
	}  
	else
	{
	    return 0;
	}   
}

sbit buzzer= P2^3;       //������ 	   

//�������ʱ�䵽�����������һ��ʱ��,�ڼ�����������¾�ֹͣ���������ʱ��Ҳֹͣ
void alarm()
{
    uint temp_1, temp_2;
    if ((alarm_flag == ON) && (adjust_ring() == 1))
	{
	    ringing_flag = ON;		
		alarm_flag = OFF;   
	}
		
    for (temp_1 = 0; temp_1 < 1000; temp_1++)
	{  
	    for (temp_2 = 0; temp_2 < 1000; temp_2++)
		{
		    if (ringing_flag == ON)
			{
			    buzzer = 0; 
			}
			else
			{
			    buzzer = 1;	  
				ringing_flag = OFF;	 
				break; 
			}	
		}
		
		if (ringing_flag == OFF)
		{
		    break;
		}		   
	} 
	
	buzzer = 1;	  	    //���һֱû�м������£�������Զ�ֹͣ��
	ringing_flag = OFF;	 
}

//----------------�������㱨ʱ������---------------------//
bit whole_speak_flag =OFF;    //�Ƿ��������㱨ʱ�ı�־

//���㱨ʱ����������ÿ����һ�ξͱ�ʱһ��
static void speak_once()
{
    buzzer = 0;
	delay(400, 400);
	buzzer = 1;
}

//���㱨ʱ����
void whole_speak()
{
    static uchar speak_times = 100;     //��ʱ�Ĵ���������100�����ֵ��Ϊ�˲�����������������ͬ
    if ((whole_speak_flag == ON) && (show_minute == 0) && (speak_times == 100)) 
	{
	    if (show_hour == 0)
		{
		    speak_times = 24;
		}
		else
		{
		    speak_times = show_hour;
		}
	}

	if ((whole_speak_flag == ON) && (speak_times != 100))
	{
	    if (speak_times > 0)
		{
		    speak_once();
			speak_times--;
		}
		else
		{
		    speak_times = 100;
		}
	}
}

















