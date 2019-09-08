#include "command.h"
#include "string.h"
#include "usart.h"
#include "Data_Processing.h"
#include "sht20.h"


Content contenT;

/****************************************   向模组发at指令，并对返回数据判断  *************************************************************/
int mqtt_send(char data[],char *flag){
	HAL_UART_Transmit(&huart3,(uint8_t *)data,strlen(data),0xff);
	printf("%s",data);
	HAL_Delay(200);
	if(UsartType.RX_flag)    	// Receive flag
    {
			printf("UsartType.RX_flag:%d\r\n",UsartType.RX_flag);
			printf("UsartType.RX_pData:%s\r\n",UsartType.RX_pData);
			UsartType.RX_flag=0;	// clean flag
			
			find_command((char *)UsartType.RX_pData,"creq");                      //寻找平台下发命令      
			if(strstr((char *)UsartType.RX_pData,flag)){
				memset(&UsartType.RX_pData,0,strlen((char *)UsartType.RX_pData)); //清空字符串
				return 1;
			}
			else{
				memset(&UsartType.RX_pData,0,strlen((char *)UsartType.RX_pData));   //清空字符串
				return 0;
			} 
    }
		return 0;

	
}

/****************************************   将原始数据转换成16进制  *************************************************************/
void rawdata_hex(char data[]){
		char test1[200],test2[200],test3[200],test4[4];
		SHT20_GetValue(&contenT.temp,&contenT.hum);        //获取温度湿度
		sprintf(test1,"{\"temperature\":%.2f,\"humidity\":%.2f}",contenT.temp,contenT.hum); //没转换为16进制的数据
		sprintf(test4,"00%X",strlen(test1));  //数据包长度转16进制

		strToHex(test1,test2);  //数据包转16进制
		sprintf(test3,"03%s%s",test4,test2);    //数据包+数据包长度 -》 16进制
		sprintf(data,"%s%d,%s\r\n",nb_mqttput,strlen(test1)+3,test3);	
}

/****************************************   向模组发at指令，连接onenet  *************************************************************/
int connect_onenet(void){
		int time_at=0;
		mqtt_send(nb_reseet,"OK");
		HAL_Delay(5000);
	
		while(mqtt_send(nb_at,"OK")==0){         //发at指令，50次错误。退出
			mqtt_send(nb_at,"OK");
			time_at++;
			if(time_at>50){return 0;}
		}
		
		if(mqtt_send(nb_haeder,"OK")==0){    //配置mqtt基础
			HAL_Delay(300);
			mqtt_send(nb_haeder,"OK");
		}
		HAL_Delay(3000);
		if(mqtt_send(nb_mqtt_key,"539068867")==1){  //查看mqtt基础配置
			mqtt_send(nb_connect,"OK");
			return 1;
		}
		else{return 0;}
}

int reconnection(void){ //重连接
	int time=0;
	if(mqtt_send(nb_mqtt_state,"1")==1 || mqtt_send(nb_mqtt_state,"2")==1){    //1 已初始化,未连接
			HAL_Delay(500);
			if(mqtt_send(nb_connect,"OK")==1)
				return 1;
			else return 0;
	}
	else if(mqtt_send(nb_mqtt_state,"3")==0 || mqtt_send(nb_mqtt_state,"4")==0){
			HAL_Delay(3000);
		while(mqtt_send(nb_mqtt_state,"5")==0){   //网络延时过久，重启
			connect_onenet();
			HAL_Delay(500);
			time++;
			if(time>10){
				return 0;          //重连失败
			}
		}
		return 1;
	}
	return 0;
}


void find_command(char *raw_data,char *flag){
		char *p=NULL;
		if(strstr((char *)UsartType.RX_pData,flag)){  //判断是否有下发命令
			p=strtok((char *)UsartType.RX_pData,",");
			while(p){
				if(strstr(p,"111")){
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
					HAL_Delay(500);
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
					break;
				}
				else{
					p=strtok(NULL,",");
				}
			}	
		}
}	

