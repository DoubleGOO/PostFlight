#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "wifi.h"

u8 req;
u16 ptc08head[2]={0x56,0x00};
u16 ptc08init[2]={0x26,0x00};
u16 ptc08take[3]={0x36,0x01,0x00};
u16 ptc08readlen[3]={0x34,0x01,0x00};
u16 ptc08readphoto[4]={0x32,0x0c,0x00,0x0a};
u16 ptc08clear[3]={0x36,0x01,0x03};
u16 ptc08cmp[7]={0x31,0x05,0x01,0x01,0x12,0x04,0x50};
u16 ptc08size[7]={0x31,0x05,0x04,0x01,0x00,0x19,0x11};//320*240
u16 ptc08bound[5]={0x24,0x03,0x01,0x0d,0xa6};//115200


void PTC08_init(void);
void PTC08_SendHead(void);
void PTC08_SendInit(void);
void PTC08_SendTake(void);
void PTC08_SendReadLen(void);
void PTC08_SendReadPhoto(void);
void PTC08_SendClear(void);
void PTC08_SendCmp(void);
void PTC08_SendSize(void);
void PTC08_SendBound(void);

u16 photolen = 0;

int main(void)
{
	u8 len;
	u8 len2;
	u8 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	uart1_init(115200);
	uart2_init(115200);
	Initial_UART4(115200);
	Initial_System_Timer();
	esp8266_init();
	PTC08_init();
	//while(1){
	  PTC08_SendClear();
		PTC08_SendTake();
		PTC08_SendReadLen();
		PTC08_SendReadPhoto();
	//}
}

void PTC08_init()
{
	delay_ms(2500);
	PTC08_SendInit();
	PTC08_SendBound();
	PTC08_SendCmp();
	PTC08_SendSize();
	PTC08_SendClear();
	PTC08_SendInit();
}

void PTC08_SendHead()
{
	for(int i = 0; i < 2; i++){
		uart4_putchar(ptc08head[i]);
	}
}

void PTC08_SendInit()
{
	u8 success = 0;
	PTC08_SendHead();
	for(int i = 0; i < 2; i++){
		uart4_putchar(ptc08init[i]);
	}
	delay_ms(500);
	if(UART4_buffer[0] == 0x76 && UART4_buffer[1] == 0x00 && UART4_buffer[2] == 0x26 && UART4_buffer[3] == 0x00){
		printf("初始化成功\r\n");
	}
	else
		printf("初始化失败\r\n");
	Flag = 0;
}

void PTC08_SendTake()
{
	PTC08_SendHead();
	for(int i = 0; i < 3; i++){
		uart4_putchar(ptc08take[i]);
	}
	delay_ms(300);
	if(UART4_buffer[0] == 0x76 && UART4_buffer[1] == 0x00 && UART4_buffer[2] == 0x36 && UART4_buffer[3] == 0x00 && UART4_buffer[4] == 0x00){
	}
	else
		printf("拍照启动失败\r\n");
	Flag = 0;
}

void PTC08_SendReadLen()
{
	PTC08_SendHead();
	for(int i = 0; i < 3; i++){
		uart4_putchar(ptc08readlen[i]);
	}
	delay_ms(300);
	if(UART4_buffer[0] == 0x76 && UART4_buffer[1] == 0x00 && UART4_buffer[2] == 0x34 && UART4_buffer[3] == 0x00 && UART4_buffer[4] == 0x04 && UART4_buffer[5] == 0x00 && UART4_buffer[6] == 0x00){
		photolen = (UART4_buffer[7] << 8) | UART4_buffer[8];
	}
	else{}
	Flag = 0;
}

void PTC08_SendReadPhoto()
{
	PTC08_SendHead();
	for(int i = 0; i < 4; i++){
		uart4_putchar(ptc08readphoto[i]);
	}
	uart4_putchar(0x00);
	uart4_putchar(0x00);
	uart4_putchar(0x00);
	uart4_putchar(0x00);
	uart4_putchar(0x00);
	uart4_putchar(0x00);
	uart4_putchar((u8)(photolen>>8));
	uart4_putchar((u8)(photolen&0x00ff));
	delay_ms(500);
	int i = 5;
	if(UART4_buffer[0] == 0x76 && UART4_buffer[1] == 0x00 && UART4_buffer[2] == 0x32 && UART4_buffer[3] == 0x00 && UART4_buffer[4] == 0x00){
		while(UART4_buffer[i] != 0xff || UART4_buffer[i] !=0xd9){
			uart2_putchar(UART4_buffer[i++]);
		}
	}
	Flag = 0;
}

void PTC08_SendClear()
{
	PTC08_SendHead();
	for(int i = 0; i < 3; i++){
		uart4_putchar(ptc08clear[i]);
	}
	delay_ms(500);
	if(UART4_buffer[0] == 0x76 && UART4_buffer[1] == 0x00 && UART4_buffer[2] == 0x36 && UART4_buffer[3] == 0x00 && UART4_buffer[4] == 0x00){
		printf("缓存清空成功\r\n");
	}
	else
		printf("缓存清空失败\r\n");
	Flag = 0;
}

void PTC08_SendCmp()
{
	PTC08_SendHead();
	for(int i = 0; i < 7; i++){
		uart4_putchar(ptc08cmp[i]);
	}
	delay_ms(500);
	if(UART4_buffer[0] == 0x76 && UART4_buffer[1] == 0x00 && UART4_buffer[2] == 0x31 && UART4_buffer[3] == 0x00 && UART4_buffer[4] == 0x00){
		printf("压缩率设置成功\r\n");
	}
	else
		printf("压缩率设置失败\r\n");
	Flag = 0;
}

void PTC08_SendSize()
{
	PTC08_SendHead();
	for(int i = 0; i < 7; i++){
		uart4_putchar(ptc08size[i]);
	}
	delay_ms(500);
	if(UART4_buffer[0] == 0x76 && UART4_buffer[1] == 0x00 && UART4_buffer[2] == 0x31 && UART4_buffer[3] == 0x00 && UART4_buffer[4] == 0x00){
		printf("图片大小设置成功\r\n");
	}
	else
		printf("图片大小设置失败\r\n");
	Flag = 0;
}

void PTC08_SendBound()
{
	PTC08_SendHead();
	for(int i = 0; i < 5; i++){
		uart4_putchar(ptc08bound[i]);
	}
	delay_ms(500);
	if(UART4_buffer[0] == 0x76 && UART4_buffer[1] == 0x00 && UART4_buffer[2] == 0x24 && UART4_buffer[3] == 0x00 && UART4_buffer[4] == 0x00){
		printf("波特率设置成功\r\n");
	}
	else
		printf("波特率设置失败\r\n");
	Flag = 0;
}

