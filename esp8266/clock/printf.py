from dictionary import dicts

class Printf:

    def __init__(self,oled):
        self.oled=oled
        self.clear()
    def clear(self):#清屏
        self.oled.fill(0)
        self.oled.show()

    def en(self, String, x, y):#显示英文，oled：屏幕对象，string：英文内容，x，y：屏幕坐标
        self.oled.text(String, x, y)
        self.oled.show()

    def printf(self,char,x_axis,y_axis,line=30):#line字体行数（高）
        offset_ = 0
        a=['','','','','']
        for k in char:#汉字和数字，英文的16进制列数不同
            if k in ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9',' ',':']:
                code = (hex(ord(k))[2:] + " ").replace(' ', '')
                byte_data = dicts[code]
            else:
                code = 0x00  # 将中文转成16进制编码
                data_code = k.encode("utf-8")
                code |= data_code[0] << 16
                code |= data_code[1] << 8
                code |= data_code[2]
                byte_data = dicts[code]
            for y in range(0, line):  # 控制y轴
                for i in range(0,int((len(byte_data)/line))):#x轴的循环次数
                    a[i] = bin(byte_data[y+i*line]).replace('0b', '')#二进制换算
                    while len(a[i]) < 8:  # 控制x轴
                        a[i] = '0' + a[i]
                    for x in range(0, 8):#填充像素块
                        # pass
                        self.oled.pixel(x_axis + offset_ + x+i*8, y + y_axis, int(a[i][x]))  # 对8个像素点处理
            if k in [':','.']:
                offset_ += 6  # 让字体横着显示，控制间距
            else:
                offset_ += line
            self.oled.show()

#
# p1=Printf('oled')
# p1.printf('123    ',1,1)
