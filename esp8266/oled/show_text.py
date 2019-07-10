from fonts import font

def clear():#清屏
	oled.fill(0)
    oled.show()


def english(oled, String, x, y):#显示英文，oled：屏幕对象，string：英文内容，x，y：屏幕坐标
    oled.fill(0)
    oled.show()
    oled.text(String, x, y)
    oled.show()

def chinese(oled, ch_str, x_axis, y_axis):#显示中文，oled：屏幕对象，string：中文内容，x，y：屏幕坐标
    oled.fill(0)
    oled.show()#清屏
    offset_ = 0
    y_axis = y_axis * 8  # 中文高度一行占8个
    x_axis = (x_axis * 16)  # 中文宽度占16个
    for k in ch_str:
        code = 0x00  # 将中文转成16进制编码
        data_code = k.encode("utf-8")
        code |= data_code[0] << 16
        code |= data_code[1] << 8
        code |= data_code[2]
        byte_data = font[code]
        for y in range(0, 16):
            a_ = bin(byte_data[y]).replace('0b', '')
            while len(a_) < 8:
                a_ = '0' + a_
            b_ = bin(byte_data[y + 16]).replace('0b', '')
            while len(b_) < 8:
                b_ = '0' + b_
            for x in range(0, 8):
                oled.pixel(x_axis + offset_ + x, y + y_axis, int(a_[x]))
                oled.pixel(x_axis + offset_ + x + 8, y + y_axis, int(b_[x]))
        offset_ += 16
    oled.show()