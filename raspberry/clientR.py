# coding:utf-8
import socket
import sys
import time
import serial
import numpy
import pyrealsense2 as rs
# Import Numpy for easy array manipulation
import numpy as np
# Import OpenCV for easy image rendering
import cv2
import threading

# Create a pipeline

# ip = '192.168.137.1'

def reconnect(address):
    sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    try:
        sock.connect(address)

    except:
        pass
    return sock

class realsense_socket(object):
    def __init__(self, ip):
        self.ip = ip
        self.pipeline = rs.pipeline()
        # Create a config and configure the pipeline to stream
        #  different resolutions of color and depth streams
        config = rs.config()

        # Get device product line for setting a supporting resolution
        pipeline_wrapper = rs.pipeline_wrapper(self.pipeline)
        pipeline_profile = config.resolve(pipeline_wrapper)
        device = pipeline_profile.get_device()
        device_product_line = str(device.get_info(rs.camera_info.product_line))

        config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)

        if device_product_line == 'L500':
            config.enable_stream(rs.stream.color, 960, 540, rs.format.bgr8, 30)
        else:
            config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

        # Start streaming
        profile = self.pipeline.start(config)

        # Getting the depth sensor's depth scale (see rs-align example for explanation)
        depth_sensor = profile.get_device().first_depth_sensor()
        depth_scale = depth_sensor.get_depth_scale()
        print("Depth Scale is: ", depth_scale)

        # We will be removing the background of objects more than
        #  clipping_distance_in_meters meters away
        clipping_distance_in_meters = 1  # 1 meter
        clipping_distance = clipping_distance_in_meters / depth_scale

        # Create an align object
        # rs.align allows us to perform alignment of depth frames to others frames
        # The "align_to" is the stream type to which we plan to align depth frames.
        align_to = rs.stream.color
        self.align = rs.align(align_to)
    def SendVideo(self):
        # 建立sock连接
        # address要连接的服务器IP地址和端口号
        address = (self.ip, 8003)
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # 开启连接
            sock.connect(address)
        except socket.error as msg:
            print(msg)
            sock = reconnect(address)

        encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 95]

        while True:
            time.sleep(0.01)
            # 建立图像读取对象
            frames = self.pipeline.wait_for_frames()
            # frames.get_depth_frame() is a 640x360 depth image

            # Align the depth frame to color frame
            aligned_frames = self.align.process(frames)

            color_frame = aligned_frames.get_color_frame()

            color_image = np.asanyarray(color_frame.get_data())
            # Validate that both frames are valid
            result, imgencode = cv2.imencode('.jpg', color_image, encode_param)
            # 建立矩阵
            data = numpy.array(imgencode)
            stringData = data.tostring()
            try:
                sock.send(str.encode(str(len(stringData)).ljust(16)));
                sock.send(stringData);
                receive = sock.recv(1024)
            except:
                sock = reconnect(address)
            # if len(receive): print(str(receive, encoding='utf-8'))
            # 读取下一帧图片

            #if cv2.waitKey(10) == 27:
            #    break
        sock.close()

    def senddep(self):
        address = (self.ip, 8004)
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect(address)
        except socket.error as msg:
            print(msg)
            sock = reconnect(address)

        while True:
            time.sleep(0.01)

            frames = self.pipeline.wait_for_frames()
            # frames.get_depth_frame() is a 640x360 depth image

            # Align the depth frame to color frame
            aligned_frames = self.align.process(frames)
            aligned_depth_frame = aligned_frames.get_depth_frame()
            depth_image = np.asanyarray(aligned_depth_frame.get_data())
            result, imgencode = cv2.imencode('.png', depth_image)
            # 建立矩阵
            data = numpy.array(imgencode)
            # 将numpy矩阵转换成字符形式，以便在网络中传输
            stringData = data.tostring()

            try:
                sock.send(str.encode(str(len(stringData)).ljust(16)));
                # 发送数据
                sock.send(stringData);
                # 读取服务器返回值
                receive = sock.recv(1024)
            except:
                sock = reconnect(address)
            # if len(receive): print(str(receive, encoding='utf-8'))
            # 读取下一帧图片

            #if cv2.waitKey(10) == 27:
            #   break
        sock.close()


class stm32_socket(object):
    def __init__(self, ip):
        self.ip = ip
    def start_threading(self):
        try:
            address = (self.ip,6666)
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((self.ip, 6666))
        except socket.error as msg:
            print(msg)
            s = reconnect(address)
        # print(s.recv(1024))  # 目的在于接受：Accept new connection from (...
        port = 3
        ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=2)
        while 1:
            # data = input('please input work: ').encode()
            data_read = None
           # data_read = ser.read()

            data_read = 't27.12b56.22b1000b1000b1000b30.47755b114.8585'.encode()
            # data_read = 't27.12b56.22b1000b1000b1000b30.47755b114.8585'.encode()
            try:
                s.send(data_read)
                order = s.recv(1024).decode()
                print(order)
                if order == 'e':
                    print('++')
                    ser.write(b'e\r\n')
                if order == 'a':
                    ser.write(b'a\r\n')
                if order == 'b':
                    print('---------------')
                    ser.write(b'b\r\n')
                if order == 'c':
                    ser.write(b'c\r\n')
                if order == 'd':
                    ser.write(b'd\r\n')

                if data_read == 'exit':
                    break
            except:
                s = reconnect(address)
        s.close()





class Cam_socket(object):
    def __init__(self, ip):
        self.ip = ip
    def start_threading(self):
        address = (self.ip, 8002)
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # 开启连接
            sock.connect(address)
        except socket.error as msg:
            print(msg)
            sock = reconnect(address)
        capture = cv2.VideoCapture(0)
        ret, frame = capture.read()
        encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 95]

        while ret:
            time.sleep(0.01)
            result, imgencode = cv2.imencode('.jpg', frame, encode_param)
            # 建立矩阵
            data = numpy.array(imgencode)
            stringData = data.tostring()
            try:
                sock.send(str.encode(str(len(stringData)).ljust(16)));
                # 发送数据
                sock.send(stringData);
                # 读取服务器返回值
                receive = sock.recv(1024)
                # if len(receive): print(str(receive, encoding='utf-8'))
                # 读取下一帧图片
                ret, frame = capture.read()
            except:
                sock = reconnect(address)
            # if cv2.waitKey(10) == 27:
            #   break
        sock.close()


def start_threading(ip):
    realsense = realsense_socket(ip)
    stm32 = stm32_socket(ip)
    Cam = Cam_socket(ip)

    t1 = threading.Thread(target=realsense.SendVideo)
    t2 = threading.Thread(target=Cam.start_threading)
    t3 = threading.Thread(target=stm32.start_threading)
    t4 = threading.Thread(target=realsense.senddep)
    t4.start()
    t1.start()
    t2.start()
    t3.start()
