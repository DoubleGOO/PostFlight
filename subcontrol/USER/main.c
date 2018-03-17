#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "pwm.h"

extern u8 command;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200
 	TIM12_PWM_Init((2000-1),840-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.     
	while(1)
	{
		AngleCtrl();//机械臂控制，通过串口1接收指令
	}
}


