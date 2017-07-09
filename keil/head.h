//本文件主要用作头文件，用于宏定义，,全局变量声明，函数声明等

//宏定义
#define uchar unsigned char
#define uint unsigned int
#define ON 1			//设置两个代表开关的常量，便于程序的可读性
#define OFF 0

//定义显示秒，分，小时，日，星期，月，年的全局变量
extern uint show_second;
extern uint show_minute;
extern uint show_hour;
extern uint show_day;
extern uint show_week;     
extern uint show_mouth;
extern uint show_year;		 //年份显示两位年份

//设置显示状态，按键状态的全局变量
//程序状态：0为正常，1为秒表，2为闹钟调整,3为时间调整，4为修改(闹铃，整点报时)开关
extern uchar MODE;      //显示状态，deal2.c中定义及意义

//关于秒表的设定部分，位于deal3.c中
extern bit stopwatch_flag;  //秒表开关的标志
extern uint stopwatch_hour;
extern uint stopwatch_minute;
extern uint stopwatch_second;
extern uint stopwatch_msec;   //百分之一秒

//关于闹铃部分的设定部分，位于deal3.c中
extern bit alarm_flag;   //判断闹铃是否开启的标志，0关，1开
extern uchar alarm_hour;
extern uchar alarm_minute;
extern uchar ringing_flag;	 //判断闹铃正在响的标志

//关于液晶背光部分的设定，位于lcd.c中  
#define LCD_CONSTANT 800  //液晶背光时间的常数
#define LCD_START {backlight_constant = LCD_CONSTANT; backlight_flag = 1;} 		 //按s4时启动背光
extern uint backlight_constant;	   //通过这个数的循环减少，来设置液晶亮一定时间
extern bit backlight_flag;	           //判断液晶是否处于亮的状态,1亮，0不亮

//关于整点报时的设定
extern bit whole_speak_flag;    //是否启动整点报时的标志,1开启，0关闭
 
//函数声明 
extern void lcd_init();	   //lcd.c中的函数
extern void lcd_show();	
extern void LCD_backlight();

extern void interrupt_init();   //deal.c中的函数
extern void time_date_carry();
extern uchar check_keyboard();
extern void delay(int x, int y);


extern void deal_keyboard(uchar key);  //deal2.c中的函数

extern void stopwatch_carry();	   //deal3.c中的函数
extern void stopwatch_reset();
extern void alarm();
extern void whole_speak();


