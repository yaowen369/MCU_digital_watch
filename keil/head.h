//���ļ���Ҫ����ͷ�ļ������ں궨�壬,ȫ�ֱ�������������������

//�궨��
#define uchar unsigned char
#define uint unsigned int
#define ON 1			//�������������صĳ��������ڳ���Ŀɶ���
#define OFF 0

//������ʾ�룬�֣�Сʱ���գ����ڣ��£����ȫ�ֱ���
extern uint show_second;
extern uint show_minute;
extern uint show_hour;
extern uint show_day;
extern uint show_week;     
extern uint show_mouth;
extern uint show_year;		 //�����ʾ��λ���

//������ʾ״̬������״̬��ȫ�ֱ���
//����״̬��0Ϊ������1Ϊ���2Ϊ���ӵ���,3Ϊʱ�������4Ϊ�޸�(���壬���㱨ʱ)����
extern uchar MODE;      //��ʾ״̬��deal2.c�ж��弰����

//���������趨���֣�λ��deal3.c��
extern bit stopwatch_flag;  //����صı�־
extern uint stopwatch_hour;
extern uint stopwatch_minute;
extern uint stopwatch_second;
extern uint stopwatch_msec;   //�ٷ�֮һ��

//�������岿�ֵ��趨���֣�λ��deal3.c��
extern bit alarm_flag;   //�ж������Ƿ����ı�־��0�أ�1��
extern uchar alarm_hour;
extern uchar alarm_minute;
extern uchar ringing_flag;	 //�ж�����������ı�־

//����Һ�����ⲿ�ֵ��趨��λ��lcd.c��  
#define LCD_CONSTANT 800  //Һ������ʱ��ĳ���
#define LCD_START {backlight_constant = LCD_CONSTANT; backlight_flag = 1;} 		 //��s4ʱ��������
extern uint backlight_constant;	   //ͨ���������ѭ�����٣�������Һ����һ��ʱ��
extern bit backlight_flag;	           //�ж�Һ���Ƿ�������״̬,1����0����

//�������㱨ʱ���趨
extern bit whole_speak_flag;    //�Ƿ��������㱨ʱ�ı�־,1������0�ر�
 
//�������� 
extern void lcd_init();	   //lcd.c�еĺ���
extern void lcd_show();	
extern void LCD_backlight();

extern void interrupt_init();   //deal.c�еĺ���
extern void time_date_carry();
extern uchar check_keyboard();
extern void delay(int x, int y);


extern void deal_keyboard(uchar key);  //deal2.c�еĺ���

extern void stopwatch_carry();	   //deal3.c�еĺ���
extern void stopwatch_reset();
extern void alarm();
extern void whole_speak();


