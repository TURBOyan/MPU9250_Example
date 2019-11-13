使用此推荐IO是需要注意，需要自行确认是否全部满足自己的需求，如果使用此推荐IO出现任何问题需要自行承担后果
摄像头             单片机接口
D0-D7数据口        B24 B25 B26 B27 B28 B29 B30 B31
SDA                A9   F5
SCL                A8
场信号             A0
像素信号           A1

运放
通道1              A10
通道2              A15
通道3              A16
通道4              A31
通道5              B0
通道6              A23

IIC                 
SDA                A13 F1
SCL                A14

电机PWM
PWM1               A18   T1
PWM2               A20
PWM3               B4   T2
PWM4               B5



编码器1(不支持正交解码，需要使用方向输出为高低电平的编码器，如我们店铺中的mini编码器)
脉冲引脚 LSB1      A2   T0
方向引脚 DIR       A3

编码器2(不支持正交解码，需要使用方向输出为高低电平的编码器，如我们店铺中的mini编码器)
脉冲引脚 LSB2      A4  T3
方向引脚 DIR       A5


舵机   
PWM                A6 T4

OLED/TFT
SCK                B19
MOSI               B21
MISO               B20      液晶没有这个引脚  但是SPI会占用这个引脚
CS                 B9 F4

DC                 B10
RES                B8
BL                 B7

UART                
RX                 A24
TX                 A25  

按键   
B1 B2 B3 B23任意交换  


拨码开关		
B17  B22任意交换  

NRF
B15 B14 A29 A22 A21 A17	任意交换  