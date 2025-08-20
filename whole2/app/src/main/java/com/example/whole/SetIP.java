package com.example.whole;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;

public class SetIP extends AppCompatActivity {

    public TextView text_device;
    public Button btn_setip;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_set_ip);

        text_device = findViewById(R.id.text_device);

        String device_id = getIntent().getStringExtra("device_id");
        switch (device_id)
        {
            case "robot":
                text_device.setText(device_id);
                break;
            case "laptop":
                text_device.setText(device_id);
                break;
        }





//        String data = getResources().getString(R.string.robot);
//        data = String.format();







    }















    public String[] initAssets() {
        try {
            //获取输入流
            InputStream inputStream = getAssets().open("ipport.txt");//这里的名字是你的txt 文本文件名称
            //获取学生名单
            String str = getString(inputStream);
            //字符分割 每行为一个学生
            String[] arr = str.split("\n");
            return arr;
        } catch (IOException e1) {
            e1.printStackTrace();
        }
        return null;
    }


    public static String getString(InputStream inputStream) {
        InputStreamReader inputStreamReader = null;
        try {
            inputStreamReader = new InputStreamReader(inputStream, "UTF-8");
        } catch (UnsupportedEncodingException e1) {
            e1.printStackTrace();
        }
        BufferedReader reader = new BufferedReader(inputStreamReader);
        //创建字符缓冲流
        StringBuffer sb = new StringBuffer("");
        String line;
        try {
            //读取每行学生
            while ((line = reader.readLine()) != null) {
                //添加到字符缓冲流中
                sb.append(line);
                //一条一行
                sb.append("\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return sb.toString();
    }




}
