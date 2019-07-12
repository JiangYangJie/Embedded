from machine import Pin, I2C
i2c = I2C(scl=Pin(5), sda=Pin(4))
from show_text import chinese
from ssd1306 import SSD1306_I2C 
oled= SSD1306_I2C(128, 64, i2c)

	   
chinese(oled,'01234',0,0) 
chinese(oled,'年月日',40,0) 