//
// Created by sophyda on 2021/7/6.
//
#include "com_example_whole_process_img.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <jni.h>
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <bitset>
#include <stdio.h>
#include <sys/socket.h>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_whole_process_1img_getEdge(JNIEnv *env, jobject thiz, jobject bitmap) {
    // TODO: implement getEdge()
    AndroidBitmapInfo info;
    void *pixels;

    CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
    CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
              info.format == ANDROID_BITMAP_FORMAT_RGB_565);
    CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
    CV_Assert(pixels);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        Mat gray;
        cvtColor(temp, gray, COLOR_RGBA2GRAY);
        Canny(gray, gray, 45, 75);
        cvtColor(gray, temp, COLOR_GRAY2RGBA);
    } else {
//        Mat temp(info.height, info.width, CV_8UC2, pixels);
//        Mat gray;
//        cvtColor(temp, gray, COLOR_RGB2GRAY);
//        Canny(gray, gray, 45, 75);
//        cvtColor(gray, temp, COLOR_GRAY2RGB);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_whole_process_1img_gray(JNIEnv *env, jobject thiz, jobject bitmap) {
    // TODO: implement gray()
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_whole_process_1img_contours(JNIEnv *env, jobject thiz, jobject bitmap) {
    // TODO: implement contours()
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_whole_process_1img_watershed(JNIEnv *env, jobject thiz, jobject bitmap) {
    // TODO: implement watershed()
    AndroidBitmapInfo info;
    void *pixels;

    CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
    CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
              info.format == ANDROID_BITMAP_FORMAT_RGB_565);
    CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
    CV_Assert(pixels);

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat src(info.height, info.width, CV_8UC4, pixels);
        int row = src.rows;
        int col = src.cols;
        //1. 将RGB图像灰度化
        Mat grayImage;
        cvtColor(src, grayImage, COLOR_RGBA2GRAY);
        //2. 使用大津法转为二值图，并做形态学闭合操作
        threshold(grayImage, grayImage, 0, 255, THRESH_BINARY | THRESH_OTSU);
        //3. 形态学闭操作
        Mat kernel = getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1));
        morphologyEx(grayImage, grayImage, MORPH_CLOSE, kernel);
        //4. 距离变换
        distanceTransform(grayImage, grayImage, DIST_L2, DIST_MASK_3, 5);
        //5. 将图像归一化到[0, 1]范围
        normalize(grayImage, grayImage, 0, 1, NORM_MINMAX);
        //6. 将图像取值范围变为8位(0-255)
        grayImage.convertTo(grayImage, CV_8UC1);
        //7. 再使用大津法转为二值图，并做形态学闭合操作
        threshold(grayImage, grayImage, 0, 255, THRESH_BINARY | THRESH_OTSU);

        morphologyEx(grayImage, grayImage, MORPH_CLOSE, kernel);
        //8. 使用findContours寻找marks
        vector<vector<Point>> contours;
        findContours(grayImage, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(-1, -1));
        Mat marks;
        marks = Mat::zeros(grayImage.size(), CV_32SC1);
        for (size_t i = 0; i < contours.size(); i++)
        {
            //static_cast<int>(i+1)是为了分水岭的标记不同，区域1、2、3...这样才能分割
            drawContours(marks, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i + 1)), 2);
        }
        //9. 对原图做形态学的腐蚀操作

        //10. 调用opencv的分水岭算法
        //watershed(src, marks);
        //11. 随机分配颜色
        vector<Vec3b> colors;
        for (size_t i = 0; i < contours.size(); i++) {
            int r = theRNG().uniform(0, 255);
            int g = theRNG().uniform(0, 255);
            int b = theRNG().uniform(0, 255);
            colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
        }

        cvtColor(marks,src,COLOR_GRAY2RGBA);

//
//        // 12. 显示
//        //Mat dst = Mat::zeros(marks.size(), CV_8UC4);
//        Mat dst ;
//        dst = Mat::zeros(marks.size(),CV_8UC4);
//        int index = 0;
//        for (int i = 0; i < row; i++) {
//            for (int j = 0; j < col; j++) {
//                index = marks.at<int>(i, j);
//                if (index > 0 && index <= contours.size()) {
//                    dst.at<Vec3b>(i, j) = colors[index - 1];
//                }
//                else if (index == -1)
//                {
//                    dst.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
//                }
//                else {
//                    dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
//                }
//            }
//        }
//        src = dst;
        AndroidBitmap_unlockPixels(env, bitmap);





    }
    else {
        Mat img(info.height, info.width, CV_8UC2, pixels);


    }
}