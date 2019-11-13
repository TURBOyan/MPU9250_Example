#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

//----LED引脚----	 
#define  OLED_SCL_PIN	B15
#define  OLED_SDA_PIN	A5
#define  OLED_RST_PIN	A19
#define  OLED_DC_PIN	A6
#define  OLED_CS_PIN	B9

//----MPU6050模拟IIC引脚-----
#define MPU6050_SCL_PIN     A9
#define MPU6050_SDA_PIN     A8

//----按键----
#define Button_Up    B26
#define Button_Down  B3
#define Button_Left  B2
#define Button_Right B12
#define Button_Mid   A7

//----拨码开关----
#define Switch_1 B10	//无
#define Switch_2 B23	//无
#define Switch_3 B17	//无
#define Switch_4 B1	  //无

//----蜂鸣器----
#define Beep   B9
#define Beep_Init   gpio_init(Beep,GPO,0,PULLUP)
#define Beep_On		  gpio_set(x,1)
#define Beep_Off		gpio_set(x,0)

//舵机
#define Servo B5				//引脚定义
#define Servo_Init ctimer_pwm_init(TIMER2_PWMCH0_B5, 330,2800)	//舵机初始化
#define Servo_Up   ctimer_pwm_duty(TIMER2_PWMCH0_B5, 2800)		//舵机拿起
#define Servo_Down ctimer_pwm_duty(TIMER2_PWMCH0_B5, 6000)		//舵机放下

//电磁铁
#define Elema_Mid   B7		//引脚定义
#define Elema_Left  B22
#define Elema_Right B4
#define Elema_Init(x)     gpio_init(x,GPO,0,PULLUP)	//电磁铁初始化
#define Elema_Absorb(x)   gpio_set(x,1)			//电磁铁吸起
#define Elema_Unabsorb(x) gpio_set(x,0)			//电磁铁放下


////串口
//#define Bluetooth_UART     USART_0
//#define Bluetooth_UART_TXD UART0_TX_A25
//#define Bluetooth_UART_RXD UART0_RX_A24

#endif
