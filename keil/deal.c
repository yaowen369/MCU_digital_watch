//���ļ�������Ҫ�����м����̣�ʱ�����ڵĽ�λ���жϵĳ�ʼ������ʱ����

#include<reg52.h>
#include"head.h"

//������ʾ�룬�֣�Сʱ���գ����ڣ��£����ȫ�ֱ���
uint show_second = 0;
uint show_minute = 59;
uint show_hour = 23;
uint show_day = 5;
uint show_week = 3;     
uint show_mouth = 10;
uint show_year = 2011;


//�жϳ�ʼ����ʹ��T0,T1������ʱ��
void interrupt_init()
{
    EA = 1;
	TMOD = 0x11;

	ET0 = 1;      //T0������ʽ1��50ms����ʱ��
	TR0 = 1;
	TH0 = (65535 - 46080) / 256;           	
	TL0 = (65535 - 46080) % 256;

	ET1 = 1;	  //T1������ʽ1��15msɨ�����
	TR1 = 1;
	TH1 = (65535 - 13824) / 256;        
	TL1	= (65535 - 13824) % 256;
}

//�ж�һ������Ƿ�������,0���ǣ�1��
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

//����,�·ݵĽ�λ
static void day_mouth_carry()
{
    if (MODE == 0)			 //������ʾʱ�Ľ�λ
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
	else			 //���޸�ʱ��ģʽ״̬(MODE=3)�½�λ��ʽ��ĳһλ�ϵĲ���Ӱ�쵽����λ��
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
		
		if (show_day == 33)		  //��ֹ����2�·ݴ���ʮ��֮��Ŀ�ʼ����ʱ������
		{
		    show_day = 1;
		}		 
	}
	   
}

//ʱ���Լ����ڵĽ�λ����
void time_date_carry()
{
    if (MODE == 0)	//������ʾʱ�͵�������ʱ�Ľ�λ��ʽ�ǲ�һ����
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
	else	   	//���޸�ʱ�伴MODE=3ʱ��λ��ʽ��ĳһλ�ı仯������������λ�ı仯
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

	if (show_day > 28)		  //���յ��µĽ�λ������һ������
	{
	    day_mouth_carry();
	}

    if (MODE == 0)		   //��������ʾ��MODE=0���µĽ�λ
	{
	   	if (show_mouth == 13)		   
		{
		    show_year++;
			show_mouth = 1;
		}  
	} 
	else		 //���޸�ʱ��ʱ��MODE=3��ʱ�µĽ�λ
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

//�����̺��������ذ����ĺ���
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

//��ʱ����
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





















