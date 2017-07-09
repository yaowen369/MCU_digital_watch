//���ļ�����һ���������������жϺ���

#include<reg52.h>
#include"head.h"

static uchar key_down = 0;  //���ð���״̬��ȫ�ֱ���(���ޱ��ļ�)

//������
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

		while (key_down)	    //���ⳤʱ�䰴�������
		{
		    ;
		}	
	}
}

//�ж�T0,50ms����ʱ��
void time0() interrupt 1	  //T0������ʽ1
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


//---------����Ϊ�����-------------//
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

//�ж�T1��ÿ15msɨ�����
void time1() interrupt 3	  //T1������ʽ1
{
    TH1 = (65535 - 13824) / 256;        
	TL1	= (65535 - 13824) % 256;

	key_down = check_keyboard();

	if ((ringing_flag == ON) && (key_down != 0))	    //�������������ʱ�򣬰�������һ������ֹͣ,�����������Ӱ��
	{
	    ringing_flag = OFF;
		key_down = 0;
	}

	if (backlight_flag == OFF)		           //���ޱ�������£���������һ����������ʹ���������������������Ӱ��
	{
	    if (key_down != 0)
		{
		    key_down = 4;
		} 
	}
}












