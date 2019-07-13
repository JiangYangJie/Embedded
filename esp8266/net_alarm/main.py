from machine import Pin, I2C,Timer
import time
from get_time import TIME
from printf import Printf
from ssd1306 import SSD1306_I2C
from wlan import Network
#import micropython


tim1 = Timer(-1)#初始化定时器1
tim2 = Timer(-2)
# micropython.alloc_emergency_exception_buf(100)#分配内存，用来中断
i2c = I2C(scl=Pin(5), sda=Pin(4))
oled= SSD1306_I2C(128, 64, i2c)#初始oled
Time=TIME()#初始自定义的TIME类
printf=Printf(oled)#初始自定义的输出类
network=Network()#初始自定义的联网类
#初始化引脚。其初始值为1
key_time = Pin(0, Pin.IN)
key_now = Pin(2, Pin.OUT,value=0)
flag_alarm = 0#用于检测是否有闹钟
variable_x=variable_y=0
year=month=day=hours=minute=Year=Month=Day=Hours=Minute=''

def callback1(p):#中断回调函数，更新实时时间
    global year, month, day, hours, minute
    year, month, day, hours, minute = Time.Now_time()
def callback2(p):#中断回调函数，判断闹钟是否开启
    global year, month, day, hours, minute,Year,Month,Day,Hours,Minute,flag_alarm
    Year, Month, Day, Hours, Minute =Time.Alarm_time()
    judge_alarm_time(Year)



def variable_():#实现动态效果
    global variable_x,variable_y
    if variable_x>128:
        variable_x=0
    else:
        variable_x+=10
    if variable_y>64:
        variable_y = 0
    else:
        variable_y += 10

def print_time(year, month, day, hours, minute,flag):#oled输出时间
    printf.clear()
    printf.printf(hours+':'+minute,5,0)
    if flag==1:#输出当前界面所表示的意义
        printf.en('now', 0, 40)
    else:
        printf.en('alarm', 0, 40)
    printf.en(year + '.' + month + '.' + day, 47, 40)
    # return year, month, day, hours, minute

def judge_alarm_time(Year):
    global flag_alarm
    if Year!=' ':#判断是否存在闹钟
        flag_alarm=1
    else:
        flag_alarm=0
    # return year, month, day, hours, minute
def Judge_alarm(Year,Month,Day,Hours,Minute):#判断闹钟是否开启
    global flag_alarm,year,month,day,hours,minute
    if Time.judge_time(year,month,day,hours,minute,Year,Month,Day,Hours,Minute,2):
        print('clock time')
        key_now.value(1)
        time.sleep(40)
        key_now.value(0)
        flag_alarm=0
        year, month, day, hours, minute = Time.Now_time()

#key_time.irq(trigger=Pin.IRQ_RISING , handler=callback)#将key_time加入中断

while network.is_connected()==False:#连接网络
    printf.clear()
    variable_()
    printf.en('wating...',variable_x,variable_y)
    time.sleep(3)

#初始化定时器
tim1.init(period=60000, mode=Timer.PERIODIC, callback=callback1)#开启定时器·1
tim2.init(period=60000, mode=Timer.PERIODIC, callback=callback2)#开启定时器·2
printf.clear()
printf.en('connection successful',1,30)
callback1('p')#完成实时时间的初始化
callback2('2')#网络闹钟初始化

while True:
    pin_info=key_time.value()
    if pin_info==1:#查看当前用户给的状态 1位now time
        print_time(year, month, day, hours, minute,1)
    else:
        if flag_alarm==1:#判断是否有闹钟
            print_time(Year, Month, Day, Hours, Minute,0)
        else:
            printf.clear()
            printf.en('no alarm clock',0,30)
    time.sleep(36)
    if flag_alarm==1:# 判断闹钟是否开启
        year, month, day, hours, minute = Time.Now_time()
        Judge_alarm(Year, Month, Day, Hours, Minute)
    print("flag_alarm",flag_alarm)

