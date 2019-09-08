#ifndef __COMMAND_H
#define __COMMAND_H


#define nb_at "AT\r\n"   //AT
#define nb_reseet "AT+CMRB\r\n"   //软复位
#define nb_haeder "AT+MQTTCFG=\"183.230.40.39\",6002,\"539068867\",120,\"240986\",\"=8NJpaIgf4n2dyZOP4QMTHHkRvw=\",1\r\n"    //onenet用户接入点
#define nb_mqtt_key "AT+MQTTCFG?\r\n"
#define nb_connect "AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n"           //连接onenet
#define nb_mqttput "AT+MQTTPUB=$dp,0,1,0,"             //mqtt跟新数据流的header
/*	0 未初始化
	1 已初始化
	2 已断开
	3 发送 Connect 包
	4 正在重连服务器
	5 已连接
	6 socket 连接服务器中
*/
#define nb_mqtt_state "AT+MQTTSTAT?\r\n"               //mqtt状态  
#define nb_get_topic "AT+MQTTSUB=<topic>,<qos>[,<index>]\r\n"   //mqtt订阅主题   topic-主题 qos- 订阅的消息交付质量等级，0-2
#define nb_cancel_topic "AT+MQTTUNSUB=<topic>\r\n"             //取消订阅
#define nb_disconnect "AT+MQTTDISC\r\n"            //主动断开连接
#define nb_ack "AT+MQTTPING\r\n"                 //该命令设置运行心跳包 ACK 显示，当未检测到服务器 ACK 时会引起重连  
#define nb_csq "AT+CSQ\r\n"                   //信号质量
#define nb_imei "AT+CGSN=1\r\n"              //查询IMEI编号


typedef struct {
	float temp;
	float hum;
} Content;

extern Content contenT;

extern float temperature;
extern float humidity;

/********************************* 函数  ****************************************/
int mqtt_send( char data[],char *flag);

/****************************  将数据转换成16进制    ********************************/
void rawdata_hex(char data[]);
/****************************  连接onenet    ********************************/
int connect_onenet(void);
/****************************  重连    ********************************/
int reconnection(void);
/****************************  找下发命令    ********************************/
void find_command(char *raw_data,char *flag);

#endif

