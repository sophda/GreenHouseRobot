package com.example.whole;

import androidx.appcompat.app.AppCompatActivity;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.TabHost;
import android.widget.TextView;
import android.widget.Toast;

import com.example.whole.view.DiscView;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.PortUnreachableException;
import java.net.Socket;

public class video extends AppCompatActivity {

    WebView webview;
    private static final int COMPLETED = 0x111;
    Button btn_start, btn_stop;
    public String global_action="00 00";
    private TextView textView ;
    private DiscView view;
    private MyBroadCastReceiver receiver;
    private LocalBroadcastManager manager;
    public String ip_robot;
    private BufferedReader in = null;
    private PrintWriter out = null;
    private Socket socket = null;

    public void socket_serial(char order)
    {
        new Thread()
        {
            @Override
            public void run() {
                super.run();
                try {

                    socket = new Socket("192.168.1.100", 6768);  // 约定 6768 为控制端口
                    in = new BufferedReader(new InputStreamReader(socket.getInputStream(), "UTF-8"));
                    out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(
                            socket.getOutputStream())), true);
                    if (socket.isConnected()) {

                        if (!socket.isInputShutdown()){
                            out.println(order);
                            out.close();
                            socket.close();
                        }
                    }



                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }.start();
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video);
        webview = findViewById(R.id.webview);
        btn_start = findViewById(R.id.btn_start);
        btn_stop = findViewById(R.id.btn_stop);

//        newbutton = findViewById(R.id.newweb);
        //设置WebView属性，能够执行Javascript脚本
        //webview.getSettings().setJavaScriptEnabled(true);
        WebSettings settings = webview.getSettings();
        //settings.setJavaScriptEnabled(true);
        settings.setUseWideViewPort(true);//设定支持viewport属性,有些页面是很早期的只适配了pc的浏览器的页面，页面很大，需要宽大的视野来展示，这时候设置为true就可以在足够的空间拖动，并且页面可以以任意比例缩放。
        settings.setLoadWithOverviewMode(true);   //自适应屏幕,用尽可能大的视野展示页面，一般和上一属性搭配使用。当然大视野显示的字都很小。
        settings.setBuiltInZoomControls(true); // 使用缩放控件
        settings.setDisplayZoomControls(false); // 隐藏缩放控件
        settings.setSupportZoom(true);//设定支持缩放,只设置此属性并不能实现缩放，在某些版本的手机上也只是能实现点击缩放


        webview.setWebViewClient(new WebViewClient(){
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;
            }
        });

        webview.setWebViewClient(new WebViewClient(){
            @Override
            public void  onPageStarted(WebView view, String url, Bitmap favicon) {
                //设定加载开始的操作
            }
        });
        webview.setBackgroundColor(0);
        String IP_PORT ;
        String IP_from_intent = getIntent().getStringExtra("IP");
        ip_robot = IP_from_intent;
        if (IP_from_intent!=null)
        {
            Toast.makeText(video.this,"连接至："+IP_from_intent,Toast.LENGTH_SHORT).show();
            IP_PORT = "http://"+IP_from_intent+":5000";
        }
        else {

            IP_PORT="http://192.168.43.178:5000";
        }

        webview.loadUrl(IP_PORT);
//        newbutton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
////                webview.loadUrl(IP_PORT);
//            }
//        });


        view = findViewById(R.id.view);
        textView = findViewById(R.id.textView2);

        /**
         * 注册本地广播接收器就，接收广播
         **/
        manager = LocalBroadcastManager.getInstance(this);
        IntentFilter filter = new IntentFilter();
        filter.addAction(view.DISC_BROADCAST);
        receiver = new MyBroadCastReceiver();
        manager.registerReceiver(receiver,filter);

        btn_start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                socket_serial('s');

            }
        });
        btn_stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                socket_serial('p');
            }
        });


        //  使用socket多线程 发送机器人运动指令
        new Thread()
        {

            public void run() {
                try {
                    while (true)
                    {
                        socket = new Socket("192.168.1.108", 6768);  // 约定 6768 为控制端口
                        in = new BufferedReader(new InputStreamReader(socket.getInputStream(), "UTF-8"));
                        out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(
                                socket.getOutputStream())), true);
                        if (socket.isConnected()) {

                            if (!socket.isInputShutdown() && (global_action != "00 00")){
                            out.println(global_action);
                            out.close();
                            socket.close();
                            }
                        }

                    }

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }.start();

    }

    class MyBroadCastReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            String string = intent.getStringExtra("move");
            global_action = string;
            textView.setText("坐标 "+string);

            }




        }
    }



