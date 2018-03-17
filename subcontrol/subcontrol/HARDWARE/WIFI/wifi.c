#include "wifi.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "usart.h"
//WIFIÄ£¿éÎªATK-ESP8266  IP:192.168.4.1
void esp8266_init()
{
	usart2_SendString("AT+RST\r\n");
	delay_ms(100);
	usart2_SendString("AT+CIPMUX=1\r\n");
	delay_ms(500);
	usart2_SendString("AT+CIPSERVER=1,8080\r\n");
	delay_ms(500);
}
