
 
/** 头文件包含区 ------------------------------------------------ */
#include "gpio_iic.h"
 
/** 私有宏(类型定义) -------------------------------------------- */ 
#define IIC1_SCL(pin_status)        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, pin_status);
#define IIC1_SDA(pin_status)        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, pin_status);
#define IIC1_SCL_IS_HIGH()          (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) != GPIO_PIN_RESET)
#define IIC1_SDA_IS_HIGH()          (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) != GPIO_PIN_RESET)
 
/** 私有变量 --------------------------------------------------- */
 
 
/** 外部变量 --------------------------------------------------- */
 
/** 私有函数原型 ----------------------------------------------- */
static void iic_delay(void);
 
/** 公有函数 --------------------------------------------------- */
/**
  * @brief  IIC启动
  * @param  None
  * @retval None
  * @note   当SCL处于高电平状态时，SDA出现一个下降沿
            即产生IIC启动信号
  */
void iic_start(void)
{
    IIC1_SCL(GPIO_PIN_SET);
    /** SDA产生一个下降沿 */
    IIC1_SDA(GPIO_PIN_SET);
    iic_delay(); 
    
    IIC1_SDA(GPIO_PIN_RESET);
    iic_delay(); 
    IIC1_SCL(GPIO_PIN_RESET);   /**< 拉低准备发送数据 */
    iic_delay();   
}
 
/**
  * @brief  IIC停止
  * @param  None
  * @retval None
  * @note   当SCL处于高电平状态时，SDA出现一个上升沿
            即产生IIC停止信号
  */
void iic_stop(void)
{
    IIC1_SCL(GPIO_PIN_RESET);
    iic_delay();
    /** SDA产生一个上升沿 */
    IIC1_SDA(GPIO_PIN_RESET);
    iic_delay();
    
    IIC1_SCL(GPIO_PIN_SET);
    iic_delay();
    IIC1_SDA(GPIO_PIN_SET);
    iic_delay();
}
 
/**
  * @brief  IIC发送1byte数据
  * @param  None
  * @retval None
  * @note   
  */
void iic_sendbyte(uint8_t byte)
{
    uint8_t i;
    
    /** 发送一个字节的高7位 */
    for (i = 0; i < 8; i++)
    {
        if (byte & 0x80)
        {
            IIC1_SDA(GPIO_PIN_SET);
        }
        else
        {
            IIC1_SDA(GPIO_PIN_RESET);
        }
        
        iic_delay();
        IIC1_SCL(GPIO_PIN_SET);
        iic_delay();
        IIC1_SCL(GPIO_PIN_RESET);
        
        if (i == 7)
        {
            IIC1_SDA(GPIO_PIN_SET);
        }
        
        byte <<= 1;
        iic_delay();
    }      
}
 
/**
  * @brief  IIC读取1byte数据
  * @param  None
  * @retval None
  * @note   读1个字节，ack=1时，发送ACK，ack=0，发送nACK          
  */
uint8_t iic_readbyte(unsigned char ack)
{
    uint8_t i;
    uint8_t recv_value = 0;
    
    IIC1_SDA(GPIO_PIN_SET);
    iic_delay();
    
    for (i = 0; i < 8; i++)
    {
        IIC1_SCL(GPIO_PIN_SET);
        iic_delay();
        recv_value <<= 1;
        if (IIC1_SDA_IS_HIGH())
        {
            recv_value |= 0x01;
        }
        else
        {
            recv_value &= ~0x01;
        }
        
        iic_delay();
        IIC1_SCL(GPIO_PIN_RESET);
    }
		if (!ack)
        iic_nack();//发送nACK
    else
        iic_ack(); //发送ACK   
    
    return recv_value;
}
 
/**
  * @brief  IIC等待应答信号
  * @param  None
  * @retval ack_status： 应答状态，0表示应答，1表示设备无响应
  */
uint8_t iic_wait_ack(void)
{
    uint8_t ack_status = 0;
    
    /** 在等待应答信号之前，要释放总线，即将SDA置位 */
    IIC1_SDA(GPIO_PIN_SET);
    iic_delay();
    IIC1_SCL(GPIO_PIN_SET);
    iic_delay();
    
    if (IIC1_SDA_IS_HIGH())
    {    
        ack_status = 1;
        iic_stop();
    }
    else
    {
        ack_status = 0;
    }
    
    IIC1_SCL(GPIO_PIN_RESET);
    iic_delay();
    
    return ack_status;
}
 
/**
  * @brief  主机（主控制器）产生应答信号
  * @param  None
  * @retval None
  */
void iic_ack(void)
{
    IIC1_SDA(GPIO_PIN_RESET);
    iic_delay();
    
    IIC1_SCL(GPIO_PIN_SET);
    iic_delay();
    IIC1_SCL(GPIO_PIN_RESET);
    iic_delay();
    
    IIC1_SDA(GPIO_PIN_SET);
}
 
/**
  * @brief  主机（主控制器）产生不应答信号
  * @param  None
  * @retval None
  */
void iic_nack(void)
{
    IIC1_SDA(GPIO_PIN_SET);
    iic_delay();
    
    IIC1_SCL(GPIO_PIN_SET);
    iic_delay();
    IIC1_SCL(GPIO_PIN_RESET);
    iic_delay();
}
 
/**
  * @brief  检测IIC总线上的设备状态
  * @param  device_addr： 从机设备地址 
  * @retval ack_status： 0 （正常）or 1（异常）
  * @note   主机发送设备地址等待从机应答，若有从机正确的应答信号
            则表明IIC总线上挂接了设备，否则表示IIC总线上未检测到
            设备
  */
uint8_t iic_check_device_status(uint8_t device_addr)
{
    uint8_t ack_status;
    
    if (IIC1_SCL_IS_HIGH() && IIC1_SDA_IS_HIGH())
    {
        iic_start();
        /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
			  iic_sendbyte(device_addr | 0);//IIC_WRITE==0 define不能编译成功
        ack_status = iic_wait_ack();
 
        iic_stop();
        
        return ack_status;
    }
    
    return 1;
}
 
/** 私有函数 --------------------------------------------------- */
/**
  * @brief  用于模拟IIC时的简单延时
  * @param  None
  * @retval None
  */
static void iic_delay(void)
{
    uint8_t i = 0;
    uint8_t delay = 5;
    
    while (delay--)
    {
        i = 10;
        while (i--);
    }
}

