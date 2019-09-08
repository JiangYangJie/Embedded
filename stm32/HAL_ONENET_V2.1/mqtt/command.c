#include "command.h"
#include "string.h"
#include "usart.h"
#include "Data_Processing.h"
#include "sht20.h"


Content contenT;

/****************************************   ��ģ�鷢atָ����Է��������ж�  *************************************************************/
int mqtt_send(char data[],char *flag){
	HAL_UART_Transmit(&huart3,(uint8_t *)data,strlen(data),0xff);
	printf("%s",data);
	HAL_Delay(200);
	if(UsartType.RX_flag)    	// Receive flag
    {
			printf("UsartType.RX_flag:%d\r\n",UsartType.RX_flag);
			printf("UsartType.RX_pData:%s\r\n",UsartType.RX_pData);
			UsartType.RX_flag=0;	// clean flag
			
			find_command((char *)UsartType.RX_pData,"creq");                      //Ѱ��ƽ̨�·�����      
			if(strstr((char *)UsartType.RX_pData,flag)){
				memset(&UsartType.RX_pData,0,strlen((char *)UsartType.RX_pData)); //����ַ���
				return 1;
			}
			else{
				memset(&UsartType.RX_pData,0,strlen((char *)UsartType.RX_pData));   //����ַ���
				return 0;
			} 
    }
		return 0;

	
}

/****************************************   ��ԭʼ����ת����16����  *************************************************************/
void rawdata_hex(char data[]){
		char test1[200],test2[200],test3[200],test4[4];
		SHT20_GetValue(&contenT.temp,&contenT.hum);        //��ȡ�¶�ʪ��
		sprintf(test1,"{\"temperature\":%.2f,\"humidity\":%.2f}",contenT.temp,contenT.hum); //ûת��Ϊ16���Ƶ�����
		sprintf(test4,"00%X",strlen(test1));  //���ݰ�����ת16����

		strToHex(test1,test2);  //���ݰ�ת16����
		sprintf(test3,"03%s%s",test4,test2);    //���ݰ�+���ݰ����� -�� 16����
		sprintf(data,"%s%d,%s\r\n",nb_mqttput,strlen(test1)+3,test3);	
}

/****************************************   ��ģ�鷢atָ�����onenet  *************************************************************/
int connect_onenet(void){
		int time_at=0;
		mqtt_send(nb_reseet,"OK");
		HAL_Delay(5000);
	
		while(mqtt_send(nb_at,"OK")==0){         //��atָ�50�δ����˳�
			mqtt_send(nb_at,"OK");
			time_at++;
			if(time_at>50){return 0;}
		}
		
		if(mqtt_send(nb_haeder,"OK")==0){    //����mqtt����
			HAL_Delay(300);
			mqtt_send(nb_haeder,"OK");
		}
		HAL_Delay(3000);
		if(mqtt_send(nb_mqtt_key,"539068867")==1){  //�鿴mqtt��������
			mqtt_send(nb_connect,"OK");
			return 1;
		}
		else{return 0;}
}

int reconnection(void){ //������
	int time=0;
	if(mqtt_send(nb_mqtt_state,"1")==1 || mqtt_send(nb_mqtt_state,"2")==1){    //1 �ѳ�ʼ��,δ����
			HAL_Delay(500);
			if(mqtt_send(nb_connect,"OK")==1)
				return 1;
			else return 0;
	}
	else if(mqtt_send(nb_mqtt_state,"3")==0 || mqtt_send(nb_mqtt_state,"4")==0){
			HAL_Delay(3000);
		while(mqtt_send(nb_mqtt_state,"5")==0){   //������ʱ���ã�����
			connect_onenet();
			HAL_Delay(500);
			time++;
			if(time>10){
				return 0;          //����ʧ��
			}
		}
		return 1;
	}
	return 0;
}


void find_command(char *raw_data,char *flag){
		char *p=NULL;
		if(strstr((char *)UsartType.RX_pData,flag)){  //�ж��Ƿ����·�����
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

