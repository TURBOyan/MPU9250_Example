/*!
 * @file       myiic.c
 * @brief      模拟IIC函数实现
 * @author     刘力铭  //删改：严一展2019年3月9日 21:44:31
 */
#include "SelfBuild_myiic.h"

static void IIC_Start(void);
static void IIC_Stop(void);
static uint8_t IIC_ChackAsk(void);
static void IIC_SendAsk(char NewState);
static uint8_t IIC_SendByte(uint8_t data);
static uint8_t IIC_ReadByte(char NewState);

/************************************************************************/
//函数:IIC引脚初始化函数无
//从机地址:SlaveAddress,寄存器地址:REG_Address,写入的数据:REG_data
//返回:无
/************************************************************************/
void IIC_Init(void)
{ 
	gpio_init(MY_SCL_GPIO, GPO, 1,PULLUP);
  gpio_init(MY_SDA_GPIO, GPO, 1,PULLUP);
}

/************************************************************************/
//函数:IIC发送寄存器数据
//参数:从机地址:SlaveAddress,寄存器地址:REG_Address,写入的数据:REG_data
//返回:0 = 成功;1 = 失败
/************************************************************************/
uint8_t IIC_WriteRegister(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
{
    IIC_Start();
    if(IIC_SendByte(SlaveAddress<<1|0))return 1;
    if(IIC_SendByte(REG_Address))return 1;
    if(IIC_SendByte(REG_data))return 1;
    IIC_Stop();
		return 0;
}

/************************************************************************/
//函数:IIC连续发送寄存器数据
//参数:从机地址:SlaveAddress,寄存器地址:REG_Address,
//		 len:连续发送的次数,buf:发送的数据地址
//返回:0 = 成功;1 = 失败
/************************************************************************/
uint8_t IIC_WriteRegisterLen(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf)
{
	uint8_t temp;
	IIC_Start();
	if(IIC_SendByte(SlaveAddress<<1|0))return 1;
  if(IIC_SendByte(REG_Address))return 1;
	for(temp = 0; temp < len; ++temp)
	{
		if(IIC_SendByte(buf[temp]))return 1;
	}
	IIC_Stop();
	return 0;
}

/************************************************************************/
//函数:IIC读取寄存器数据
//参数:从机地址:SlaveAddress,寄存器地址:REG_Address,data:存储读取值的地址
//返回:0 = 成功,1 = 失败
/************************************************************************/
uint8_t IIC_ReadRegister(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t *data)
{ 
    IIC_Start();
    if(IIC_SendByte(SlaveAddress<<1|0)) return 1;
    if(IIC_SendByte(REG_Address)) return 1;
    IIC_Start();
    if(IIC_SendByte(SlaveAddress<<1|1)) return 1;
    *data = IIC_ReadByte(0);
    IIC_Stop();
    return 0;
}

/************************************************************************/
//函数:IIC连续读取寄存器数据
//参数:从机地址:SlaveAddress,寄存器地址:REG_Address,
//		 len:连续读取的次数,buf:储存数据的地址
//返回:0 = 成功,1 = 失败
/************************************************************************/
uint8_t IIC_ReadRegisterLen(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf)
{
	IIC_Start();
	if(IIC_SendByte(SlaveAddress<<1|0))return 1;
	if(IIC_SendByte(REG_Address))return 1;
	IIC_Start();
	if(IIC_SendByte(SlaveAddress<<1|1)) return 1;
	while(len)
	{
		(len==1)?((*buf)=IIC_ReadByte(0)):((*buf)=IIC_ReadByte(1));
		len--;
		buf++;
	}
	IIC_Stop();
	return 0;
}

/************************************************************************/
//函数:IIC起始信号
/************************************************************************/
static void IIC_Start(void)
{
	MY_SET_SDA_OUT;
	
  MY_SDA_H;
  MY_SCL_H;
  NOP();
  MY_SDA_L;
  NOP();
  MY_SCL_L;
}

/************************************************************************/
//函数:IIC终止信号
/************************************************************************/
static void IIC_Stop(void)
{
	MY_SET_SDA_OUT;
	
	MY_SCL_L;
	MY_SDA_L;
	NOP();
	MY_SCL_H;
	NOP();
	MY_SDA_H;
	NOP();
}

/************************************************************************/
//函数:IIC检查从机应答信号
//返回:0 = 成功;1 = 失败
/************************************************************************/
static uint8_t IIC_ChackAsk(void)
{
	uint8_t ErrTime = 0;
	MY_SET_SDA_IN;
	
//	MY_SDA_H;
//	NOP();
	MY_SCL_H;
	NOP();
	while(MY_READ_SDA)
	{
		ErrTime++;
		if(ErrTime > 254)
		{
			IIC_Stop();
			return 1;
		}
	}
	MY_SCL_L;
	NOP();
	return 0;
}

/************************************************************************/
//函数:IIC发送应答信号
//参数:SET = 应答,RESET = 非应答
/************************************************************************/
static void IIC_SendAsk(char NewState)
{
	MY_SCL_L;
	MY_SET_SDA_OUT;
	
	(NewState == 1)? MY_SDA_L: MY_SDA_H;
	NOP();
	MY_SCL_H;
	NOP();
	MY_SCL_L;
	NOP();
}

/************************************************************************/
//函数:IIC发送一个字节
//参数:data,发送的字节
//返回:0 = 成功;1 = 失败
/************************************************************************/
static uint8_t IIC_SendByte(uint8_t data)
{
	uint8_t temp;
	MY_SET_SDA_OUT;
	MY_SCL_L;
	for(temp = 0; temp < 8; temp++)
	{
		(data & 0x80)? MY_SDA_H: MY_SDA_L;
		data <<= 1;
		NOP();
		MY_SCL_H;
		NOP();
		MY_SCL_L;
		NOP();
	}
	if(IIC_ChackAsk())
		return 1;
	return 0;
}

/************************************************************************/
//函数:IIC接收一个字节
//参数:SET = 返回应答信号,RESET = 返回非应答信号
//返回:读出来的字节
/************************************************************************/
static uint8_t IIC_ReadByte(char NewState)
{
	uint8_t temp = 0,data = 0;
	MY_SET_SDA_IN;
	
	for(temp = 0; temp < 8; ++temp)
	{
		MY_SCL_L;
		NOP();
		MY_SCL_H;
		data <<= 1;
		if(MY_READ_SDA)
			data++;
		NOP();
	}
	(NewState == 1)? IIC_SendAsk(1): IIC_SendAsk(0);
	return data;
}
