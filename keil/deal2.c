//���ļ�ֻ��һ���ⲿ���������̴�����void deal_keyboard(uchar key)����ʵ�ָı�ģʽ�ȸ��ֹ���

#include<reg52.h>
#include"head.h"


//����״̬��0Ϊ������1Ϊ���2Ϊ���ӵ���,3Ϊʱ�������4��ʾ����״̬
uchar MODE = 0;      //������ʾ״̬��ȫ�ֱ���
 
//���̵��������������ݰ�����״̬���ı�ģʽ���ı�ʱ�����ڵ�ȫ�ֱ���
void deal_keyboard(uchar key)
{
    static uchar set_time_place = 0;	  //MODE=3������趨ʱ�������һλ���ꡢ�¡��ա�Сʱ���Ƿ�.
	static uchar set_alarm_place = 0;     //MODE=2����£��趨����������һλ��Сʱ���Ƿ�     
	

    switch (MODE)
	{
	    case 0:				//ģʽΪ0���,������ʾ
		{
		    switch (key)
			{
			    case 0:{}	 
				break;

				case 1:	{}	
				break;

				case 2:	MODE = 4; //��ʾ����״̬				
				break;

				case 3:	MODE++;	//ģʽ��һ��תΪ���״̬				
				break;

				case 4:	LCD_START	 	
				break;

				default:
				break;
			}
		}
		break;   //case 0 (��MODE = 0)������ʾ״̬����

		case 1:			    //ģʽΪ1�������ʾ���
		{
		    switch (key)
			{
			    case 0:	{}  
				break;

				case 1:	stopwatch_flag = ~stopwatch_flag;	//��һ�ο�ʼ��ʱ���ٴΰ���ʱֹͣ,����ѭ��							         				
				break;

				case 2:		  //ֹͣ��ʱ�����ص�������ʾ״̬
				{
				    stopwatch_reset();
					MODE = 0;
				}
				break;

				case 3:			//ģʽ��1��תΪ����״̬�����Զ���������
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
		break;	 //case1����MODE=1���������ʾ��ʽ����

		case 2:			    //ģʽΪ2���������
		{
		    switch (key)
			{
			    case 0: {}		
				break;

				case 1:		   //��ѡ���ּ�һ
				{
				    switch (set_alarm_place)
					{
					    case 0: 				    //�趨�����Сʱ
							alarm_hour++;
							if (alarm_hour == 24)
							{
							    alarm_hour = 0;   
							}	
						break;

						case 1:					  //�趨����ķ���
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

				case 2:		  //���ĵ���Сʱ���߷���
				{
				    set_alarm_place++;
					if (set_alarm_place == 2)
					{
					    set_alarm_place = 0;
					}
				}
				break;

				case 3:	  MODE++;    //ģʽ��һ��תΪ���ڣ�ʱ������								 				
				break;

				case 4:	  LCD_START 								     				
				break;

				default:
				break;
			}
		}
		break;	 //case2(��MODE=2),��������״̬����

		case 3:				//ģʽΪ3���,����ʱ�䣬����
		{
		    ET0 = OFF;     //����ͣ����ʱ���T0�ж�
   
		    switch (key)
			{
			    case 0:{}		  
				break;

				case 1:			 //	��S1��ѡ���ּ�һ��
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
				break;	//case 1: (��switch (set_time_place)) ����ѡ��λ�ϣ�Сʱ���֣��յȣ���һ ����

				case 2:		 //��S2�ı����ε���Сʱ�����ӣ����ڣ��꣬�£���
				{
				    set_time_place++;
					if (set_time_place == 6)	  
					{
					    set_time_place = 0;
					}
				}
				break;

				case 3:		   // �ٰ�S3��������ʾģʽ 
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
		break;	 //case3(��MODE=3)�޸�ʱ��,����  ����

		case 4:			    //ģʽ4(��MODE=4)��	������ģʽ�°�S2���￪��״̬ģʽ
		{
		    switch (key)
			{
			    case 0:{}		
				break;

				case 1:	 alarm_flag = ~alarm_flag;// ��S1�رջ��߿������壬��֮ǰ�෴								   			
				break;

				case 2:	whole_speak_flag = ~whole_speak_flag;	 //��S2�������߹ر����㱨ʱ����֮ǰ�෴							   			
				break;

				case 3:	   MODE = 0; //�ٰ�һ�ΰ�S3����������ʾ							   			
				break;

				case 4:	   LCD_START												
				break;

				default:
				break;
			}
		}
		break;	 //ģʽ4(��MODE=4),���ã����壬���㱨ʱ������״̬  ����

		default:
		break;
	}    
} 