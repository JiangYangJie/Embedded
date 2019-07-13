import requests
class TIME:
    def __init__(self):
        self.headers={
                'api-key':'you api key',
                "User-Agent": "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36",
                 }
        self.now_time=('','','','','')
        self.alarm_time=('','','','','')

    def get_now_time(self):
        response=requests.get('http://quan.suning.com/getSysTime.do',headers=self.headers)
        time=response.json()['sysTime1']
        year=time[:4]
        month=time[4:6]
        day=time[6:8]
        hours=time[8:10]
        minute=time[10:12]
        return year,month,day,hours,minute


    def get_alarm_time(self,year,month,day,hours,minute):
        try:#从onenet获取时间
            Time=requests.get('http://api.heclouds.com/devices/'your id'/datastreams/time',headers=self.headers)
            print(Time)
            Year=Time.json()['data']['current_value'][:4]
            Month = Time.json()['data']['current_value'][5:7]
            Day=Time.json()['data']['current_value'][8:10]
            Hours=Time.json()['data']['current_value'][11:13]
            Minute=Time.json()['data']['current_value'][14:]
            print(Year,Month,Day,Hours,Minute)
            if self.judge_time(year,month,day,hours,minute,Year,Month,Day,Hours,Minute,flag=1):
                self.alarm_time=Year,Month,Day,Hours,Minute
            else:
                self.alarm_time = '1 ',' ',' ',' ',' '
        except:
            print('aa')
            self.alarm_time = ' ',' ',' ',' ',' '

    def judge_time(self,year,month,day,hours,minute,Year,Month,Day,Hours,Minute,flag=1):
        #小写是实时时间，大写是规定时间
        if flag==1:#判断是否符合闹钟条件
            if year==Year:
                if month==Month:
                    if day==Day:
                        if hours==Hours:
                            if minute<=Minute:
                                return True
                            else:
                                return False
                        elif hours<Hours:
                            return True
                        else:
                            return False
                    elif day<Day:
                        return True
                    else:
                        return False
                elif month<Month:
                    return True
                else:
                    return True
            else:
                return False
        else:#判断是否开启闹钟
            if  minute==Minute and hours==Hours and day==Day and month==Month and year==Year:
                return True
            else:
                return False
    def Now_time(self):
        try:
            self.get_now_time()
            self.now_time =self.get_now_time()
            return self.now_time
        except:
            return ' ',' ',' ',' ',' '
    def Alarm_time(self):
        now=self.get_now_time()
        self.get_alarm_time(now[0],now[1],now[2],now[3],now[4])
        if self.alarm_time[1]!=' ':
            return self.alarm_time
        else:
            return ' ',' ',' ',' ',' '
#
# Time=TIME()
# # print(Time.Now_time(),
# # Time.Alarm_time())
# print(Time.judge_time('2019','07','11','08','39','2019','07','11','08','39',2))
# year,month,day,hours,minute=Time.get_now_time()
# print(Time.get_alarm_time(year,month,day,hours,minute))
# headers={
#                 'api-key':'3vUCCcUopoOkPuu0ZCsMSUvfyOw=',
#                 "User-Agent": "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36",
#                  }
# Time = requests.get('http://api.heclouds.com/devices/532724988/datastreams/time', headers=headers)
# print(Time)