import sys,cv2
import numpy as np
import matplotlib.pyplot as plt
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QWidget,QTextEdit
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.Qt import QEvent
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import pyqtSignal,pyqtSlot,Qt
from PyQt5 import QtWidgets,QtGui
from PyQt5.QtWidgets import QDialog,QWidget, QApplication,QMainWindow, QFileDialog
from PyQt5.uic import loadUi
from detect import Ui_MainWindow


class Detection(QMainWindow,Ui_MainWindow):
    openButton = pyqtSignal()
    def __init__(self):
        #self.img =[]
        super(Detection,self).__init__()
        self.setupUi(self)

        #loadUi('detect.ui',self)

        self.openButton.clicked.connect(self.openSlot)
        self.saveButton.clicked.connect(self.saveSlot)
        #self.modelButton.clicked.connect(self.modelSlot)
        self.detectButton.clicked.connect(self.detectSlot)

    def openSlot(self):
        '''
        从本地读取图片
        '''
        # 打开文件选取对话框
        global fname
        filename, _ = QFileDialog.getOpenFileName(self, '打开图片')
        if filename:
            self.image = cv2.imread(str(filename))
            # OpenCV图像以BGR通道存储，显示时需要从BGR转到RGB
            self.image = cv2.cvtColor(self.image, cv2.COLOR_BGR2RGB)

            rows, cols, channels = self.image.shape
            bytesPerLine = channels * cols
            QImg = QImage(self.image.data, cols, rows, bytesPerLine, QImage.Format_RGB888)
            self.Inputlabel.setPixmap(QPixmap.fromImage(QImg).scaled(
                self.Inputlabel.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
            fname = filename

    def modelSlot(self):
        """从系统中选择权重文件"""
        file = QFileDialog.getOpenFileName(self, "Pre-trained YOLOv5 Weights", "./",
                                           "Weights Files (*.pt);;All Files (*)")
        if file[0] != '':
            self.line_weights.setText(file[0])
            GLOBAL.record_config({'weights': file[0]})

    def detectSlot(self):
        from centernet1 import CenterNet
        from PIL import Image
        listz = []
        import os
        import glob
        centernet = CenterNet()
        global fname
        image = Image.open(fname)
        #self.image = self.image.convert("RGB")
        #self.image1 = centernet.detect_image(self.image)
        self.image1,listx,listy = centernet.detect_image(image)
        self.image1.save("img.jpg")
        self.image2 = cv2.imread("img.jpg")
        # OpenCV图像以BGR通道存储，显示时需要从BGR转到RGB
        self.image2 = cv2.cvtColor(self.image2, cv2.COLOR_BGR2RGB)
        rows, cols, channels = self.image2.shape
        bytesPerLine = channels * cols
        QImg = QImage(self.image2.data, cols, rows, bytesPerLine, QImage.Format_RGB888)
        self.outlabel.setPixmap(QPixmap.fromImage(QImg).scaled(
            self.outlabel.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
        #print(label)
        def compute_heigh_strong(a, b, depth_scale=1000):
            depth_map = cv2.imread("E:\\mushuai\\data\\data\\shuai\\depth4-2.png", -1)  # 读入一张深度图
            depth_cam_matrix = np.array([[501.5344, 0, 523.1547], [0, 501.7068, 509.7322], [0, 0, 1]])  # 标定后得到内参矩阵
            fx, fy = depth_cam_matrix[0, 0], depth_cam_matrix[1, 1]
            cx, cy = depth_cam_matrix[0, 2], depth_cam_matrix[1, 2]  # 获取内置参数
            h, w = np.mgrid[0:depth_map.shape[0], 0:depth_map.shape[1]]
            # print(h)
            z = depth_map / depth_scale
            x = (w - cx) * z / fx
            y = (h - cy) * z / fy  ####减掉cx,cy,偏移量，再根据深度信息确定世界坐标系的x，y坐标0
            # print(y)
            return z[a, b]  # 输出a的值

        for m in range(len(listx)):
            a = listx[m]
            b = listy[m]
            z = compute_heigh_strong(a, b, depth_scale=1000)
            listz.append(z)
        f = "E:\\mushuai\\centernet-pytorch\\point.txt"
        with open(f, "w") as file:  # ”w"代表着每次运行都覆盖内容
            for k in range(len(listx)):
                file.write(str(listx[k]) + " " + str(listy[k]) + "\n")
        for i in range(len(listx)):
            xlabel = str(listx[i])
            ylabel = str(listy[i])
            zlabel = str(listz[i])
            self.textEdit.append("num{0}".format(i+1) + ":" + xlabel + "," + ylabel+ "," + zlabel)
            #self.textEdit.append(ylabel)
        #print(len(label))
        #self.textEdit.append(len(label))


    def saveSlot(self):
        # 调用存储文件dialog
        fileName, tmp = QFileDialog.getSaveFileName(
            self, 'Save Image', './__data', '*.png *.jpg *.bmp', '*.png*.jpg *.bmp')
        if fileName is '':
            return
        if self.image.size == 1:
            return

        # 调用opencv写入图像
        self.image2 = cv2.cvtColor(self.image2, cv2.COLOR_BGR2RGB)
        cv2.imwrite(fileName, self.image2)


if __name__ == '__main__':
    app = 0
    app=QApplication(sys.argv)
    window=Detection()
    window.show()
    #window.jishuButton.clicked.connect(window.loginEvent)
    sys.exit(app.exec_())

