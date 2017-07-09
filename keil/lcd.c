//���ļ�����LCD��ȫ����غ���������������ģʽ����ʾ��ʽ�������ڲ����������������ͨ��һ���ܵ�lcd_show���������ļ�����
//���ɱ������ļ����õ���lcd_inint()(LCD��ʼ������)��LCD_backlight()(LCD�ı��⺯��)

#include<reg52.h>
#include<string.h>
#include"head.h"

#define LINE_1  0x80
#define LINE_2  0xc0

sbit RS = P2^5;		   //lcd�Ŀ����ź�
sbit RW = P2^6;
sbit E = P2^7;
sbit IS_BUSY = P0^7;

//LCD�ļ���Ƿ�æ�ĺ�����
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

//LCD��дָ���
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

//LCD��д���ݺ���
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

//LCD���������������������趨
static void clear()
{
    write_command(0x01);
	write_command(0x06);
	write_command(0x0c);
}

//-----------------------------��ʾ�õ�ʱ������,��������-----------------------//
static uchar code number_list[10] = {"0123456789"};
static uchar code week_list[7][4]={"Sun ","Mon ","Tues","Wed ","Thur","Fri ","Sat "};

//ģʽ0������ʾ �� ģʽ3�޸�ʱ�� ����ʾ��ʽ����һ��ʱ�䣬�ڶ�����ʾ����	
static void show_tim_day_mode()
{   
    char str[] = "set";
    uint i, size;
 
//----��һ��(ģʽ0)������ʾСʱ���֣���,���ڣ�ģʽ������ʾСʱ���� ����-------//
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

	write_data('\x20');	   //�ո�
	write_data('\x20');
	for (i = 0; i < 4; i++)
	{
	    write_data(week_list[show_week][i]);
	}


//--------------�ڶ���(ģʽ0����ģʽ3)��ʾ����λ���꣺�£��գ�ģʽ3����ʾset-----------------------//
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
	    write_data('\x20');		   //�ո�
		write_data('\x20');
	    size = strlen(str);
        for (i = 0; i < size; i++)
		{
		    write_data(str[i]);
		}
	}
}

//���ģʽʱ����ʾ��ʽ����case1
static void show_stopwatch_mode()
{	  
	uint i, size;  
	char string[] = "stopwatch:s1 set ";

    write_command(LINE_1);     //��һ��������ʾСʱ,��,�룬ʮ��֮һ��

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
		  
	size = strlen(string);			 //�ڶ�����ʾstopweatch mode 
	write_command(LINE_2);						  
	

	for (i = 0; i < size; i++)
	{
	    write_data(string[i]);
	}
			   

}

//�趨����ʱ����ʾ��ʽ����case2
static void show_alarm_mode()
{
    uint i, size;  
	char str_1[] = "alarm mode";
	char str_2[]= "s1:add  s2:place";

    write_command(LINE_1);     //��һ��������ʾСʱ,��,	alarm mode

    write_data(number_list[alarm_hour / 10]);
	write_data(number_list[alarm_hour % 10]);
	write_data(':');

	write_data(number_list[alarm_minute / 10]);
	write_data(number_list[alarm_minute % 10]); 
	
	size = strlen(str_1);			 //��һ�дӵ�������ʼ��ʾ  alarm mode
	write_command(0x86);						  
	

	for (i = 0; i < size; i++)
	{
	    write_data(str_1[i]);
	}  

	size = strlen(str_2);			 //�ڶ�����ʾs1:place   s2:add
	write_command(LINE_2);						  
	

	for (i = 0; i < size; i++)
	{
	    write_data(str_2[i]);
	}  
}

//���ÿ���ʱ����ʾ��ʽ����case4
static void show_on_off_mode()
{
    uint i, size;  	
	char str_1[] = "alarm:";
	char str_2[]= "Whole Speak:";
	char str_on[] = "on";
	char str_off[] = "off";	

    static bit alarm_temp = ON;   //�������������㱨ʱ��״̬���ˣ�������һ��
	static bit speak_temp = ON;
    if ((alarm_flag != alarm_temp) || (whole_speak_flag != speak_temp))
	{
	    clear();
	    alarm_temp = alarm_flag;   
	    speak_temp = whole_speak_flag;
	}

//---------------��һ����ʾ������Сʱ�����ӣ�alarm:on/off-------------//
    write_command(LINE_1);     

    write_data(number_list[alarm_hour / 10]);
	write_data(number_list[alarm_hour % 10]);
	write_data(':');

	write_data(number_list[alarm_minute / 10]);
	write_data(number_list[alarm_minute % 10]); 
	
	size = strlen(str_1);			 //��һ�дӵ�������ʼ��ʾ  alarm 
	write_command(0x86);						  
	for (i = 0; i < size; i++)
	{
	    write_data(str_1[i]);
	}

	if (alarm_flag == ON) 		   //��һ�дӵ�ʮ������ʼ��ʾon����off
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
//----------------�ڶ�����ʾwhole speak:on/off------------------//
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

//lcd��ʼ������
void lcd_init()
{
    write_command(0x38);      //������ʾģʽ�����ء��������
	delay(20, 20);
	write_command(0x0c);
	delay(20, 20);
	write_command(0x06);    
}

//LCD������ʾ����
void lcd_show()
{
    static uchar temp = 0;	    //�����ʾ��ģʽ�ı䣬������һ��
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

//-------------����Һ�����������-----------------------//
sbit backlight= P2^4;		 //Һ������
uint backlight_constant = LCD_CONSTANT;	  //ͨ���������ѭ�����٣�������Һ����һ��ʱ��
bit backlight_flag = OFF;	           //�ж�Һ���Ƿ�������״̬

//LCD���⺯�������ֱ�����һ��ʱ��
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

