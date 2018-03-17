#include "pwm.h"
#include "delay.h"
u16 comp;
u8 command;

void TIM12_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//(普通输出模式)
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12);//GPIOF14复用映射到TIM12，作为TIM12它的输出引脚；
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12);//GPIOF115复用映射到TIM12，作为TIM12它的输出引脚；
	
	TIM_TimeBaseInitStructure.TIM_Period = arr;//预装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//比较输出极性(有效的电平)
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	
	TIM_OC1Init(TIM12,&TIM_OCInitStructure);//TIM12通道1初始化函数
	TIM_OC2Init(TIM12,&TIM_OCInitStructure);//TIM12通道2初始化函数
	
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  //使能TIM12在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);
 
	TIM_ARRPreloadConfig(TIM12,ENABLE);//ARPE使能预装载
	TIM_Cmd(TIM12,ENABLE);  //使能TIM12
	
}


void Arm_Angle(double DU)
{
		comp = (u16)(10 * (DU / 9)) + 50;
		TIM_SetCompare1(TIM12,comp-1);//目标占空比换算成转换度数
}

void Hand_Angle(double DU)
{
		comp = (u16)(10 * (DU / 9)) + 50;
		TIM_SetCompare2(TIM12,comp-1);//目标占空比换算成转换度数
}

void AngleCtrl()
{
	switch(command){
		case 0x61://抬起
			Arm_Angle(0);
			break;
		case 0x62://放下
			Arm_Angle(90);
		  break;
		case 0x63://张开
			Hand_Angle(120);
			break;
		case 0x64://合拢
			Hand_Angle(180);
			break;
		default://默认姿态
			Arm_Angle(0);
			Hand_Angle(120);
			break;
	}
	delay_ms(200);
}
