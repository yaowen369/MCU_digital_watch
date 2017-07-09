//本文件只有一个外部函数，键盘处理函数void deal_keyboard(uchar key)，来实现改变模式等各种功能

#include<reg52.h>
#include"head.h"


//程序状态：0为正常，1为秒表，2为闹钟调整,3为时间调整，4显示开关状态
uchar MODE = 0;      //设置显示状态的全局变量
 
//键盘的主处理函数，根据按键的状态来改变模式，改变时间日期等全局变量
void deal_keyboard(uchar key)
{
    static uchar set_time_place = 0;	  //MODE=3情况下设定时间的是哪一位，年、月、日、小时还是分.
	static uchar set_alarm_place = 0;     //MODE=2情况下，设定调整的是哪一位，小时还是分     
	

    switch (MODE)
	{
	    case 0:				//模式为0情况,正常显示
		{
		    switch (key)
			{
			    case 0:{}	 
				break;

				case 1:	{}	
				break;

				case 2:	MODE = 4; //显示开关状态				
				break;

				case 3:	MODE++;	//模式加一，转为秒表状态				
				break;

				case 4:	LCD_START	 	
				break;

				default:
				break;
			}
		}
		break;   //case 0 (即MODE = 0)正常显示状态结束

		case 1:			    //模式为1情况，显示秒表
		{
		    switch (key)
			{
			    case 0:	{}  
				break;

				case 1:	stopwatch_flag = ~stopwatch_flag;	//按一次开始计时，再次按下时停止,依次循环							         				
				break;

				case 2:		  //停止计时，返回到正常显示状态
				{
				    stopwatch_reset();
					MODE = 0;
				}
				break;

				case 3:			//模式加1，转为闹铃状态，且自动开启闹铃
				{
				    stopwatch_reset();
				    MODE++;    
				}
				break;

				case 4:	LCD_START							     
				break;

				default:
				break;
			}
		}
		break;	 //case1（即MODE=1），秒表显示方式结束

		case 2:			    //模式为2情况，闹铃
		{
		    switch (key)
			{
			    case 0: {}		
				break;

				case 1:		   //所选数字加一
				{
				    switch (set_alarm_place)
					{
					    case 0: 				    //设定闹铃的小时
							alarm_hour++;
							if (alarm_hour == 24)
							{
							    alarm_hour = 0;   
							}	
						break;

						case 1:					  //设定闹铃的分钟
						    alarm_minute++;
							if (alarm_minute == 60)
							{
							    alarm_minute = 0;
							}	 
						break;
							
						default: 
						break;							
					}
				}
				break;	  

				case 2:		  //更改调整小时或者分钟
				{
				    set_alarm_place++;
					if (set_alarm_place == 2)
					{
					    set_alarm_place = 0;
					}
				}
				break;

				case 3:	  MODE++;    //模式加一，转为日期，时间设置								 				
				break;

				case 4:	  LCD_START 								     				
				break;

				default:
				break;
			}
		}
		break;	 //case2(即MODE=2),闹铃设置状态结束

		case 3:				//模式为3情况,调整时间，日期
		{
		    ET0 = OFF;     //先暂停驱动时间的T0中断
   
		    switch (key)
			{
			    case 0:{}		  
				break;

				case 1:			 //	按S1所选数字加一。
				{
				    switch (set_time_place)
					{
					    case 0: show_hour++;  	     
						break;

						case 1: show_minute++;    
						break;

						case 2:	 show_week++;
						break;

						case 3: show_year++;
						break;

						case 4: show_mouth++;
						break;

						case 5: show_day++;
						break;

						default:
						break;						
					}
				}
				break;	//case 1: (即switch (set_time_place)) 在所选的位上（小时，分，日等）加一 结束

				case 2:		 //按S2改变依次调整小时，分钟，星期，年，月，日
				{
				    set_time_place++;
					if (set_time_place == 6)	  
					{
					    set_time_place = 0;
					}
				}
				break;

				case 3:		   // 再按S3到正常显示模式 
				{
				    MODE = 0;
					set_time_place = 0;
					ET0 = ON;
				}
				break;

				case 4:	 LCD_START		 
				break;

				default:
				break;
			}
		}
		break;	 //case3(即MODE=3)修改时间,日期  结束

		case 4:			    //模式4(即MODE=4)，	在正常模式下按S2到达开关状态模式
		{
		    switch (key)
			{
			    case 0:{}		
				break;

				case 1:	 alarm_flag = ~alarm_flag;// 按S1关闭或者开启闹铃，与之前相反								   			
				break;

				case 2:	whole_speak_flag = ~whole_speak_flag;	 //按S2开启或者关闭整点报时，与之前相反							   			
				break;

				case 3:	   MODE = 0; //再按一次按S3返回正常显示							   			
				break;

				case 4:	   LCD_START												
				break;

				default:
				break;
			}
		}
		break;	 //模式4(即MODE=4),设置（闹铃，整点报时）开关状态  结束

		default:
		break;
	}    
} 