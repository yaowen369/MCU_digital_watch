//本文件包括LCD的全部相关函数，设置了四种模式的显示方式（都是内部函数），并且最后通过一个总的lcd_show来供其他文件调用
//还可被其他文件调用的是lcd_inint()(LCD初始化函数)和LCD_backlight()(LCD的背光函数)

#include<reg52.h>
#include<string.h>
#include"head.h"

#define LINE_1  0x80
#define LINE_2  0xc0

sbit RS = P2^5;		   //lcd的控制信号
sbit RW = P2^6;
sbit E = P2^7;
sbit IS_BUSY = P0^7;

//LCD的检测是否忙的函数，
static void adjust_busy()
{
    uchar temp = 1;
	uint i = 100;
		
	while (temp && i)
	{
	    P0 = 0xff;
		E = 0;
		delay(7, 7);
		RS = 0;
		RW = 1; 
		delay(5, 5);
		E = 1;
		delay(5, 5);
		temp = IS_BUSY;
		delay(15, 15);
		E = 0;  
		i--; 
	}
}

//LCD的写指令函数
static void write_command(int com)
{
    adjust_busy();
	E = 0;
	RS = 0;
	RW = 0;
	P0 = com;
	delay(5, 5);
	E = 1;
	delay(25, 25);   
	E = 0; 
}

//LCD的写数据函数
static void write_data(int dat)
{
    adjust_busy();
	E = 0;
	RS = 1;
	RW = 0;
	P0 = dat;
	delay(5, 5);
	E = 1;
	delay(25, 25);   
	E = 0; 
}

//LCD的清屏函数，并且重新设定
static void clear()
{
    write_command(0x01);
	write_command(0x06);
	write_command(0x0c);
}

//-----------------------------显示用的时间数组,星期数组-----------------------//
static uchar code number_list[10] = {"0123456789"};
static uchar code week_list[7][4]={"Sun ","Mon ","Tues","Wed ","Thur","Fri ","Sat "};

//模式0正常显示 或 模式3修改时间 的显示方式，第一行时间，第二行显示日期	
static void show_tim_day_mode()
{   
    char str[] = "set";
    uint i, size;
 
//----第一行(模式0)依次显示小时：分：秒,星期（模式三）显示小时：分 星期-------//
    write_command(LINE_1);     

    write_data(number_list[show_hour / 10]);
	write_data(number_list[show_hour % 10]);
	write_data(':');

	write_data(number_list[show_minute / 10]);
	write_data(number_list[show_minute % 10]);
	
	if (MODE == 0)							 
	{
	    write_data(':');
		write_data(number_list[show_second / 10]);
		write_data(number_list[show_second % 10]);
	}

	write_data('\x20');	   //空格
	write_data('\x20');
	for (i = 0; i < 4; i++)
	{
	    write_data(week_list[show_week][i]);
	}


//--------------第二行(模式0或者模式3)显示（二位）年：月：日，模式3多显示set-----------------------//
    write_command(LINE_2);     

	write_data(number_list[show_year / 1000]);
	write_data(number_list[(show_year % 1000) / 100]);
    write_data(number_list[(show_year %100) / 10]);
	write_data(number_list[(show_year % 100) %10]);
	write_data('-');

	write_data(number_list[show_mouth / 10]);
	write_data(number_list[show_mouth % 10]);
    write_data('-');

	write_data(number_list[show_day / 10]);
	write_data(number_list[show_day % 10]);   
	
	if (MODE == 3)							 
	{	
	    write_data('\x20');		   //空格
		write_data('\x20');
	    size = strlen(str);
        for (i = 0; i < size; i++)
		{
		    write_data(str[i]);
		}
	}
}

//秒表模式时的显示方式，即case1
static void show_stopwatch_mode()
{	  
	uint i, size;  
	char string[] = "stopwatch:s1 set ";

    write_command(LINE_1);     //第一行依次显示小时,分,秒，十分之一秒

    write_data(number_list[stopwatch_hour / 10]);
	write_data(number_list[stopwatch_hour % 10]);
	write_data(':');

	write_data(number_list[stopwatch_minute / 10]);
	write_data(number_list[stopwatch_minute % 10]);
	write_data(':');

	write_data(number_list[stopwatch_second / 10]);
	write_data(number_list[stopwatch_second % 10]);
    write_data(':');

    write_data(number_list[stopwatch_msec ]);
		  
	size = strlen(string);			 //第二行显示stopweatch mode 
	write_command(LINE_2);						  
	

	for (i = 0; i < size; i++)
	{
	    write_data(string[i]);
	}
			   

}

//设定闹铃时的显示方式，即case2
static void show_alarm_mode()
{
    uint i, size;  
	char str_1[] = "alarm mode";
	char str_2[]= "s1:add  s2:place";

    write_command(LINE_1);     //第一行依次显示小时,分,	alarm mode

    write_data(number_list[alarm_hour / 10]);
	write_data(number_list[alarm_hour % 10]);
	write_data(':');

	write_data(number_list[alarm_minute / 10]);
	write_data(number_list[alarm_minute % 10]); 
	
	size = strlen(str_1);			 //第一行从第六个开始显示  alarm mode
	write_command(0x86);						  
	

	for (i = 0; i < size; i++)
	{
	    write_data(str_1[i]);
	}  

	size = strlen(str_2);			 //第二行显示s1:place   s2:add
	write_command(LINE_2);						  
	

	for (i = 0; i < size; i++)
	{
	    write_data(str_2[i]);
	}  
}

//设置开关时的显示方式，即case4
static void show_on_off_mode()
{
    uint i, size;  	
	char str_1[] = "alarm:";
	char str_2[]= "Whole Speak:";
	char str_on[] = "on";
	char str_off[] = "off";	

    static bit alarm_temp = ON;   //如果闹铃或者整点报时的状态变了，就清屏一次
	static bit speak_temp = ON;
    if ((alarm_flag != alarm_temp) || (whole_speak_flag != speak_temp))
	{
	    clear();
	    alarm_temp = alarm_flag;   
	    speak_temp = whole_speak_flag;
	}

//---------------第一行显示，闹铃小时，分钟，alarm:on/off-------------//
    write_command(LINE_1);     

    write_data(number_list[alarm_hour / 10]);
	write_data(number_list[alarm_hour % 10]);
	write_data(':');

	write_data(number_list[alarm_minute / 10]);
	write_data(number_list[alarm_minute % 10]); 
	
	size = strlen(str_1);			 //第一行从第六个开始显示  alarm 
	write_command(0x86);						  
	for (i = 0; i < size; i++)
	{
	    write_data(str_1[i]);
	}

	if (alarm_flag == ON) 		   //第一行从第十二个开始显示on或者off
	{
	    for (i = 0; i < 2; i++)
		{
		    write_data(str_on[i]);
		}
	}
	else 
	{
	    for (i = 0; i < 3; i++)
		{
		    write_data(str_off[i]);
		}   
	}
//----------------第二行显示whole speak:on/off------------------//
	size = strlen(str_2);			 
	write_command(LINE_2);						  
	

	for (i = 0; i < size; i++)
	{
	    write_data(str_2[i]);
	}  

   	if (whole_speak_flag == ON) 		  
	{
	    for (i = 0; i < 2; i++)
		{
		    write_data(str_on[i]);
		}
	}
	else 
	{
	    for (i = 0; i < 3; i++)
		{
		    write_data(str_off[i]);
		}   
	}
}

//lcd初始化函数
void lcd_init()
{
    write_command(0x38);      //设置显示模式、开关、光标设置
	delay(20, 20);
	write_command(0x0c);
	delay(20, 20);
	write_command(0x06);    
}

//LCD的总显示函数
void lcd_show()
{
    static uchar temp = 0;	    //如果显示的模式改变，则清屏一次
	if (temp != MODE)
	{
	    clear();
		temp = MODE;
	}

    switch (MODE)
	{
	    case 0: show_tim_day_mode();
		break;

		case 1:	
		{
			show_stopwatch_mode();
			stopwatch_carry(); 
		}
		break;

		case 2:	 show_alarm_mode();
		break;

		case 3:	show_tim_day_mode();	
		break;

		case 4: show_on_off_mode();
		break;

		default: 
		break;

	}

}

//-------------关于液晶背光的设置-----------------------//
sbit backlight= P2^4;		 //液晶背光
uint backlight_constant = LCD_CONSTANT;	  //通过这个数的循环减少，来设置液晶亮一定时间
bit backlight_flag = OFF;	           //判断液晶是否处于亮的状态

//LCD背光函数，保持背光亮一定时间
void LCD_backlight()
{
    if (backlight_constant > 0)
	{
	    backlight_constant--;
		backlight = 0;
		backlight_flag = ON;
	}
	else
	{
	    backlight = 1;
		backlight_flag = OFF;
	}
}

