//本文件包括一个主函数，两个中断函数

#include<reg52.h>
#include"head.h"

static uchar key_down = 0;  //设置按键状态的全局变量(仅限本文件)

//主函数
void main()
{
    interrupt_init();
	lcd_init();

	while(1)
	{
	    LCD_backlight();
	    time_date_carry();	
		lcd_show();	

		whole_speak();
		alarm();
		deal_keyboard(key_down);

		while (key_down)	    //避免长时间按键的情况
		{
		    ;
		}	
	}
}

//中断T0,50ms驱动时间
void time0() interrupt 1	  //T0工作方式1
{
    static uint time0_count = 0;
    TH0 = (65535 - 46080) / 256;           	
	TL0 = (65535 - 46080) % 256;

	time0_count++;
	
	if (time0_count == 20)
	{
	    time0_count = 0;
	    show_second++;	 

		if (show_second == 60)
		{
		    show_second = 0;
			show_minute++;
		}
	}					  


//---------以下为秒表部分-------------//
    if (stopwatch_flag == ON)
	{

	    if ((time0_count % 2) == 0)
		{
		    stopwatch_msec++;
	
			if (stopwatch_msec == 10)
			{
			    stopwatch_msec = 0;
				stopwatch_second++;
			}
		}
	
	}
}

//中断T1，每15ms扫描键盘
void time1() interrupt 3	  //T1工作方式1
{
    TH1 = (65535 - 13824) / 256;        
	TL1	= (65535 - 13824) % 256;

	key_down = check_keyboard();

	if ((ringing_flag == ON) && (key_down != 0))	    //当闹铃正在响的时候，按下任意一个键就停止,不会造成其他影响
	{
	    ringing_flag = OFF;
		key_down = 0;
	}

	if (backlight_flag == OFF)		           //在无背光情况下，按下任意一个键，都会使背光亮，而不会造成其他影响
	{
	    if (key_down != 0)
		{
		    key_down = 4;
		} 
	}
}












