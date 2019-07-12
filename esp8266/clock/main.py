from machine import Pin, I2C
import time
from get_time import TIME
from printf import Printf
from ssd1306 import SSD1306_I2C
from wlan import Network

i2c = I2C(scl=Pin(5), sda=Pin(4))
oled= SSD1306_I2C(128, 64, i2c)#初始oled
Time=TIME()#初始自定义的TIME类
printf=Printf(oled)#初始自定义的输出类
network=Network()#初始自定义的联网类
variable_x=variable_y=0

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

while network.is_connected()==False:
    printf.clear()
    variable_()
    printf.en('wating...',variable_x,variable_y)
    time.sleep(3)
printf.en('connection successful',1,30)
while True:
    year, month, day, hours, minute=Time.Now_time()
    printf.clear()
    printf.printf(hours+':'+minute,5,0)
    printf.en(year + '.' + month+'.'+day, 40, 40)
    time.sleep(60)
