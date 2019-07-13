# -*- coding: utf-8 -*-

"""
Module implementing alarm.
"""

from PyQt5.QtCore import pyqtSlot, QTimer
from PyQt5.QtWidgets import QMainWindow, QMessageBox, QDockWidget
from PyQt5 import QtWidgets
from Ui_alarm import Ui_alarm
import requests, time, random


class alarm(QDockWidget, Ui_alarm):
    """
    Class documentation goes here.
    """
    def __init__(self, parent=None):
        """
        Constructor
        
        @param parent reference to the parent widget
        @type QWidget
        """
        super(alarm, self).__init__(parent)
        self.setupUi(self)
        self.timer = QTimer()
        self.timer.timeout.connect(self.Time)
        self.timer.start(10000)  # start(2000)表示设定时间间隔为2秒
        self.headers={
        'api-key':'3vUCCcUopoOkPuu0ZCsMSUvfyOw=',
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.110 Safari/537.36",
         }
    def Time(self):
        t=time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        self.lineEdit.setText(t[:4])
        self.lineEdit_2.setText(t[5:7])
        self.lineEdit_3.setText(t[8:10])
        self.lineEdit_4.setText(t[11:13])
        self.lineEdit_5.setText(t[14:16])
    def Send(self, body):
        try:
            a=requests.post('http://api.heclouds.com/devices/532724988/datapoints',json=body,headers=self.headers, timeout=2)
            a.json()
        except:
            QMessageBox.warning(self,
                                "警告",
                                "网络异常",
                                QMessageBox.Yes)
    def get_alarm_time(self):
        year=str(self.spinBox.value())
        month=str(self.spinBox_2.value())
        day=str(self.spinBox_3.value())
        hour=str(self.spinBox_4.value())
        minute=str(self.spinBox_5.value())
        if len(month)<2:
            month='0'+month
        if len(day)<2:
            day='0'+day
        if len(hour)<2:
            hour='0'+hour
        if len(minute)<2:
            minute='0'+minute
        info=year+'_'+month+'_'+day+'_'+hour+'_'+minute
        return info
    @pyqtSlot()
    def on_pushButton_clicked(self):
        """
        Slot documentation goes here.
        """
        flag=self.comboBox.currentIndex()
        print(self.get_alarm_time())
        if flag==0:
            body={'datastreams':[{'id':'time','datapoints':[{'value':self.get_alarm_time()}]}]}
            self.Send(body)
            QMessageBox.information(self,  # 使用infomation信息框
                                "结果",
                                "更新成功",
                                QMessageBox.Yes)
        else:
            self.Send(body={'datastreams':[{'id':'time','datapoints':[{'value':'1999_01_01_00_00'}]}]})
    
    @pyqtSlot()
    def on_pushButton_2_clicked(self):
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        sys.exit(app.exec_())
        
        
if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)
    ui = alarm()
    ui.show()
    sys.exit(app.exec_())
