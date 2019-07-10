#用来在ssd1306上显示


from machine import Pin, SPI
from show_text import chinese
prst = machine.Pin(5, machine.Pin.OUT)
pdc = machine.Pin(16, machine.Pin.OUT)
pcs = machine.Pin(10, machine.Pin.OUT)
spi = SPI(-1, baudrate=100000, polarity=1, phase=0, sck=Pin(0), mosi=Pin(2), miso=Pin(4))
spi.init(baudrate=200000)
from ssd1306 import Print_SPI
ssd = Print_SPI(128, 64, spi, pdc, prst, pcs)

chinese(ssd,'江阳杰',0,0)

