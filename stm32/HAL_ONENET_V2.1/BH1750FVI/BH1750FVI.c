
#include "gpio_iic.h"

#include "BH1750FVI.h"

void BH1750_start(void){
	Single_Write_BH1750(0x01);
}

void Single_Write_BH1750(uchar REG_Address)
{
   iic_start();                  //起始信号
   iic_sendbyte(BHAddWrite);   //发送设备地址+写信号
   iic_sendbyte(REG_Address);    //内部寄存器地址，
  //  BH1750_SendByte(REG_data);       //内部寄存器数据，
   iic_stop();                   //发送停止信号
		bh_data_send(BHPowOn);   
		bh_data_send(BHReset);
		bh_data_send(BHModeL); //选择的是分辨率是4lx，测量时间是16ms的
}


void bh_data_send(uint8_t command)
{
    do{
		iic_start();                      //iic起始信号
    iic_sendbyte(BHAddWrite);       //发送器件地址
    }
		while(iic_wait_ack());           //等待从机应答
    iic_sendbyte(command);          //发送指令
    iic_wait_ack();                   //等待从机应答
    iic_stop();                       //iic停止信号
}

uint16_t bh_data_read(void)
{
	uint16_t buf;
	iic_start();                       //iic起始信号
	iic_sendbyte(BHAddRead);         //发送器件地址+读标志位
	iic_wait_ack();                     //等待从机应答
	buf=iic_readbyte(1);              //读取数据
	buf=buf<<8;                        //读取并保存高八位数据
	buf+=0x00ff&iic_readbyte(0);      //读取并保存第八位数据
	iic_stop();                        //发送停止信号 
	return buf; 
}