#ifndef __SELFBUILD_MYIIC_H
#define __SELFBUILD_MYIIC_H

#include "headfile.h"

#define MY_SCL_GPIO   MPU6050_SCL_PIN 
#define MY_SDA_GPIO   MPU6050_SDA_PIN

#define NOP()   for(int tempdely = 0; tempdely < 2; ++tempdely);//增加IIC通讯的延时时间，提高稳定性

#define MY_SCL_H                        gpio_set(MY_SCL_GPIO, 1)
#define MY_SCL_L                        gpio_set(MY_SCL_GPIO, 0)

#define MY_SDA_H                        gpio_set(MY_SDA_GPIO, 1)
#define MY_SDA_L                        gpio_set(MY_SDA_GPIO, 0)

#define MY_READ_SDA                     gpio_get(MY_SDA_GPIO)

#define MY_SET_SDA_IN                   gpio_init(MY_SDA_GPIO, GPI, 1,PULLUP)
#define MY_SET_SDA_OUT                  gpio_init(MY_SDA_GPIO, GPO, 0,PULLDOWN)

#define MY_SET_SCL_IN                   gpio_ddr(MY_SCL_GPIO, GPI)
#define MY_SET_SCL_OUT                  gpio_ddr(MY_SCL_GPIO, GPO)

void IIC_Init(void);
uint8_t IIC_ReadRegister(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t *data);
uint8_t IIC_ReadRegisterLen(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf);
uint8_t IIC_WriteRegister(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data);
uint8_t IIC_WriteRegisterLen(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t len, uint8_t *buf);

#endif
