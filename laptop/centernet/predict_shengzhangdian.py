'''
predict.py有几个注意点
1、无法进行批量预测，如果想要批量预测，可以利用os.listdir()遍历文件夹，利用Image.open打开图片文件进行预测。
2、如果想要保存，利用r_image.save("img.jpg")即可保存。
3、如果想要获得框的坐标，可以进入detect_image函数，读取top,left,bottom,right这四个值。
4、如果想要截取下目标，可以利用获取到的top,left,bottom,right这四个值在原图上利用矩阵的方式进行截取。
'''
from centernet import CenterNet
from PIL import Image
import os
import glob
centernet = CenterNet()
'''
while True:
    img = input('Input image filename:')
    try:
        image = Image.open(img)
    except:
        print('Open Error! Try again!')
        continue
    else:
        r_image = centernet.detect_image(image)
        # r_image.save("img.jpg")
        r_image.show()
'''
path = "D:\\huihuipython\\centernet-pytorch\\img\\"
save_path=path+"\\detect{0}\\".format(3)  #保存的路径
if not os.path.exists(save_path):
    os.makedirs(save_path)
for img in glob.glob("D:\\huihuipython\\centernet-pytorch\\img\\detect2\\*.jpg"): #对需要裁剪的图片的文件夹循环读取
    print(img)


image =Image.open('d3.jpg')
image = image.convert("RGB")
r_image = centernet.detect_image(image)
# _, img = os.path.split(img)
#r_image.show()
r_image.save('resuly.jpg')
print(1000000)