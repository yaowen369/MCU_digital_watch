//本文件主要是处理闹铃，整点报时，秒表三项功能

#include<reg52.h>
#include"head.h"

//-------关于秒表部分,秒表精确到十分之一秒-------------------//
bit stopwatch_flag = OFF;    //秒表是否开启的标志
uint stopwatch_hour = 0;
uint stopwatch_minute = 0;
uint stopwatch_second = 0;
uint stopwatch_msec = 0;   //百分之一秒

//秒表的进位，秒进位到分,分进位到小时
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

//当不用秒表，即电子表模式不为1时，秒表的各个全局变量的复位
void stopwatch_reset()
{
    stopwatch_flag = OFF;    
    stopwatch_hour = 0;
    stopwatch_minute = 0;
    stopwatch_second = 0;
    stopwatch_msec = 0;   
}

//--------------关于闹铃部分-------------------------//
bit alarm_flag = OFF;   //判断闹铃是否开启的标志，0关，1开
uchar alarm_hour = 0;
uchar alarm_minute = 0;
uchar ringing_flag = OFF;	 //判断闹铃正在响的标志

//判断是否闹铃时间到达
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

sbit buzzer= P2^3;       //蜂鸣器 	   

//如果闹铃时间到，则蜂鸣器响一段时间,期间有任意键按下就停止，过了这段时间也停止
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
	
	buzzer = 1;	  	    //如果一直没有键被按下，则最后自动停止响
	ringing_flag = OFF;	 
}

//----------------关于整点报时的设置---------------------//
bit whole_speak_flag =OFF;    //是否启动整点报时的标志

//整点报时发生函数，每调用一次就报时一次
static void speak_once()
{
    buzzer = 0;
	delay(400, 400);
	buzzer = 1;
}

//整点报时函数
void whole_speak()
{
    static uchar speak_times = 100;     //报时的次数，赋予100这个初值是为了不与后面的整点数数相同
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

















