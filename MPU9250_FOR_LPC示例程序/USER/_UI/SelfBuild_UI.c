#include "SelfBuild_UI.h"

int16 Speed_Set;

extern uint16 Stop_Time;
struct Wireless_Flag_Typedef Wireless_Flag;		//无线调试器标志位
struct Wireless_ReceiveData_Typedef Wile_RciveData;

static inline void Flag_Turn(uint8 *Flag);


void wireless_CommunInit(void)
{
	uart_init(Bluetooth_UART, 115200, Bluetooth_UART_TXD, Bluetooth_UART_RXD);
  if(!gpio_get(Switch_3))
  {
	 uart_rx_irq(Bluetooth_UART,1);
	 uart_putchar(Bluetooth_UART, 0xff); //从串口丢一个字节数据出去
	 uart_putchar(Bluetooth_UART, 0xff); //从串口丢一个字节数据出去
	 uart_putchar(Bluetooth_UART, 0xff); //从串口丢一个字节数据出去
	 Wireless_Flag.Wirelesson_Flag=1;
	 Stop_Time=0;
  }
  else
  {
	 ALL_Run();
  	 Wireless_Flag.Wirelesson_Flag=0;
  }
}

void wireless_CommunSend(uint8 *data)
{
	uart_putstr(Bluetooth_UART , data);  
}

void wireless_Control(void)
{
  if(Wireless_Flag.Wirelesson_Flag == 0)
	{
		return;
	}
	if(Wireless_Flag.FLAG_START)
	{
	  	Wireless_Flag.FLAG_START=0;
			enable_irq(RIT_IRQn);  //开启直立控制
	}
	if(Wireless_Flag.FLAG_ALLSTOP)
	{
		Error_Flag.Wireless_Stop=1;
	}
	if(Wireless_Flag.FLAG_GO)
	{
		Wireless_Flag.FLAG_GO=0;
		Motor_Data.Speed_Ave_Set=Speed_Set;
	}
	if(Wireless_Flag.FLAG_BACK)
	{
	  Wireless_Flag.FLAG_BACK=0;
		Motor_Data.Speed_Ave_Set=-Speed_Set;
	}
	if(Wireless_Flag.FLAG_Speedzero)
	{
	  Wireless_Flag.FLAG_Speedzero=0;
		Motor_Data.Speed_Ave_Set=0;
	}
}


void FLEXCOMM0_DriverIRQHandler(void)
{
    vuint32 flag;
    flag = UART0_FIFO_FLAG;
    if(flag & USART_FIFOINTSTAT_RXLVL_MASK)//接收FIFO达到设定水平（库默认设定水平 当接收FIFO有一个数据的时候触发中断）
    {
        uart_getchar(Bluetooth_UART, &Wile_RciveData.Cmd);
				switch(Wile_RciveData.Cmd)
				{
				case 0x00:Wireless_Flag.FLAG_ALLSTOP=1;break;
				case 0x01:Wireless_Flag.FLAG_START=1;break;
				case 0x02:Wireless_Flag.FLAG_GO=1;break;
				case 0x03:Wireless_Flag.FLAG_BACK=1;break;
				case 0x04:Flag_Turn(&Wireless_Flag.FLAG_AdcCal);break;
				case 0x05:Wireless_Flag.FLAG_Speedzero=1;break;
				case 0x06:break;
				case 0x07:break;
				case 0x08:break;
				case 0x09:break;
				case 0x0A:break;
				default: break;
				}
		
#if BUZZER_EN
				gpio_set(Beep, 1);
    		pit_delay_ms(PIT1,100);
				gpio_set(Beep, 0);
#endif
		}
    
    if(flag & USART_FIFOINTSTAT_RXERR_MASK)//接收FIFO错误
    {
        USART0->FIFOCFG  |= USART_FIFOCFG_EMPTYRX_MASK;//清空RX FIFO
        USART0->FIFOSTAT |= USART_FIFOSTAT_RXERR_MASK;
    }
}

static inline void Flag_Turn(uint8 *Flag)
{
  *Flag=*Flag?0:1;
}

void Computer_Send(void)
{
	long computer[7];		//上位机参数储存
	computer[0]=AD_Data.AD_R;
	computer[1]=AD_Data.AD_R_R;
	computer[2]=AD_Data.AD_M;
	computer[3]=AD_Data.AD_L_L;
	computer[4]=AD_Data.AD_L;
	computer[5]=Motor_Data.Speed_Ave_Set;
	computer[6]=Motor_Data.Speed_Ave_Now;
//	computer[0]=adc_once(AD_CH_R_1, ADC_8bit);
//	computer[1]=adc_once(AD_CH_R__, ADC_8bit);	
//	computer[2]= adc_once(AD_CH_M_1, ADC_8bit);
//	computer[3]=adc_once(AD_CH_L__, ADC_8bit);
//	computer[4]=adc_once(AD_CH_L_1, ADC_8bit);
	vcan_sendware((uint8*)computer,sizeof(computer)); 
}

//协议的规定:每次发送数据开始都要发送两个起始位(0x4C)，再发送图片的宽和高，然后发送图像数据，但是发送数据中每当遇到和起始位(0x4C)和结束位(0x55)和数据位(0x49)时，在前面加上一个数据位，即用0x49,0x4C和0x49,0x55和0x49,0x49替换他们 
//例如：发送一个2*3数据为0x4C,0x00,0x49,0x55,0x55,0x10的图像 
//发送格式为：0x4C,0x4C,0x02,0x03,0x49,0x4C,0x00,0x49,0x49,0x49,0x55,0x49,0x55, 0x10,0x55,0x55