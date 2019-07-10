import network,time
from machine import Pin


class Network(object):

	def __init__(self):
		#配置led引脚
		self.led=Pin(16,Pin.OUT)
		#配置WiFi
		self.wlan = network.WLAN(network.STA_IF)
		self.wlan.active(True)
		print("waiting>>>>>")
		self.wlan.connect("red","123456789")#需要连接的WiFi的账号密码
	def is_connected(self):
		if self.wlan.isconnected()==True:
			return True
		else:
			return False
	def wifi_connect(self,Time):
		time.sleep(Time)
		if self.wlan.isconnected()==True:
			if self.led.value()==1:
					self.led.value(0)
			print("WiFi Connection Successful")
		else:
			print("WiFi connection failed")
			self.led_twinkle()
	def led_twinkle(self):#WiFi连接失败，模块闪烁
		for i in range(10):
			if self.led.value()==0:
				self.led.value(1)
			else:
				self.led.value(0)
			time.sleep(2)
	