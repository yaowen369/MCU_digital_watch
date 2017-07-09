//本文件函数主要功能有检测键盘，时间日期的进位，中断的初始化，延时函数

#include<reg52.h>
#include"head.h"

//定义显示秒，分，小时，日，星期，月，年的全局变量
uint show_second = 0;
uint show_minute = 59;
uint show_hour = 23;
uint show_day = 5;
uint show_week = 3;     
uint show_mouth = 10;
uint show_year = 2011;


//中断初始化，使用T0,T1两个定时器
void interrupt_init()
{
    EA = 1;
	TMOD = 0x11;

	ET0 = 1;      //T0工作方式1，50ms驱动时间
	TR0 = 1;
	TH0 = (65535 - 46080) / 256;           	
	TL0 = (65535 - 46080) % 256;

	ET1 = 1;	  //T1工作方式1，15ms扫描键盘
	TR1 = 1;
	TH1 = (65535 - 13824) / 256;        
	TL1	= (65535 - 13824) % 256;
}

//判断一个年份是否是闰年,0不是，1是
static uchar adjust_leap_year()
{
	if (((show_year % 4 == 0) && (show_year % 100 != 0)) || ((show_year % 100 == 0) && (show_year % 400 == 0)))
	{
	    return 1;
	}
	else
	{
	    return 0;
	}
}

//日期,月份的进位
static void day_mouth_carry()
{
    if (MODE == 0)			 //正常显示时的进位
	{
	    if ((show_mouth == 2) && (show_day == 29) && (adjust_leap_year() == 0))
		{
		    show_mouth = 3;
			show_day = 1;
			return ;
		}
	
		if ((show_mouth == 2) && (show_day == 30) && (adjust_leap_year() == 1))
		{
		    show_mouth = 3;
			show_day = 1;
			return ;
		}
	
		if (show_day == 31)
		{
		    if ((show_mouth == 4) ||(show_mouth == 6) || (show_mouth == 9) || (show_mouth == 11))			                        
			{
			    show_mouth++;
				show_day = 1;
				return ;
			} 
		}
	
		if (show_day == 32)
		{
		    if ((show_mouth == 1) ||(show_mouth == 3) || (show_mouth == 5) || (show_mouth == 7) 
			                        || (show_mouth == 8) || (show_mouth == 10) || (show_mouth == 12))
			{
			    show_mouth++;
				show_day = 1;
				return ;
			}
		}
	}
	else			 //在修改时间模式状态(MODE=3)下进位方式，某一位上的不会影响到其他位上
	{		
	    if ((show_mouth == 2) && (show_day == 29) && (adjust_leap_year() == 0))
		{			   
			show_day = 1;
			return ;
		}
	
		if ((show_mouth == 2) && (show_day == 30) && (adjust_leap_year() == 1))
		{
			show_day = 1;
			return ;
		}
	
		if (show_day == 31)
		{
		    if ((show_mouth == 4) ||(show_mouth == 6) || (show_mouth == 9) || (show_mouth == 11))
			                        
				{
					show_day = 1;
					return ;
				} 
		}
	
		if (show_day == 32)
		{
		    if ((show_mouth == 1) ||(show_mouth == 3) || (show_mouth == 5) || (show_mouth == 7) 
			                        || (show_mouth == 8) || (show_mouth == 10) || (show_mouth == 12))
				{
					show_day = 1;
					return ;
				}
		}
		
		if (show_day == 33)		  //防止出现2月份从三十日之类的开始调整时间的情况
		{
		    show_day = 1;
		}		 
	}
	   
}

//时间以及日期的进位函数
void time_date_carry()
{
    if (MODE == 0)	//正常显示时和调整闹铃时的进位方式是不一样的
	{		   
		if (show_minute == 60)
		{
		    show_minute = 0;
			show_hour++;
		}
	
		if (show_hour == 24)
		{
		    show_hour = 0;
		    show_day++;
			show_week++;
		}
	
		if (show_week == 7)
		{
		    show_week = 0;
		}
	} 			
	else	   	//在修改时间即MODE=3时进位方式，某一位的变化不会引起其他位的变化
	{			  		    	   
		if (show_minute == 60)
		{
		    show_minute = 0;
		}
	
		if (show_hour == 24)
		{
		    show_hour = 0;		    
		}
	
		if (show_week == 7)
		{
		    show_week = 0;
		}    		
	}

	if (show_day > 28)		  //从日到月的进位调用另一个函数
	{
	    day_mouth_carry();
	}

    if (MODE == 0)		   //在正常显示（MODE=0）月的进位
	{
	   	if (show_mouth == 13)		   
		{
		    show_year++;
			show_mouth = 1;
		}  
	} 
	else		 //在修改时间时（MODE=3）时月的进位
	{
	    if (show_mouth == 13)		   
		{
			show_mouth = 1;
		}   
	}

	if (show_year == 2100)	    
	{
	    show_year = 2000;
	}
}

//检测键盘函数，返回按键的号码
uchar check_keyboard()
{
    uchar keyboard_num;

	switch(P1)
	{
	    case 0xfe: keyboard_num = 1;
		    break;
	    case 0xfd: keyboard_num = 2;
		    break;
		case 0xfb: keyboard_num = 3;
		    break;
		case 0xf7: keyboard_num = 4;
		    break;
		default:   keyboard_num = 0;
	}

	return keyboard_num;
}

//延时函数
void delay(int x, int y)
{
    int i, j;

    for (i = 0; i < x; i++)
	{
	    for (j = 0; j < y; j++)
		{
		    ;
		}
	}
}





















