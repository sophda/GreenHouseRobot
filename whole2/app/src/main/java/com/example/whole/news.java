package com.example.whole;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.webkit.ValueCallback;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com.google.android.material.tabs.TabItem;
import com.google.android.material.tabs.TabLayout;

import java.util.Timer;
import java.util.TimerTask;

public class news extends AppCompatActivity {

    public WebView web_news;
    public TabLayout tab_web;
    private static Handler handler = new Handler();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_news);

        web_news = findViewById(R.id.web_news);
        //设置WebView属性，能够执行Javascript脚本
        web_news.getSettings().setJavaScriptEnabled(true);
        WebSettings settings = web_news.getSettings();
//        web_news.getSettings().setDomStorageEnabled(true);
        //settings.setUserAgentString("app/XXX");
//        settings.setJavaScriptEnabled(true);
        settings.setUseWideViewPort(true);//设定支持viewport属性,有些页面是很早期的只适配了pc的浏览器的页面，页面很大，需要宽大的视野来展示，这时候设置为true就可以在足够的空间拖动，并且页面可以以任意比例缩放。
        settings.setLoadWithOverviewMode(true);   //自适应屏幕,用尽可能大的视野展示页面，一般和上一属性搭配使用。当然大视野显示的字都很小。
        settings.setBuiltInZoomControls(true); // 使用缩放控件
        settings.setDisplayZoomControls(false); // 隐藏缩放控件
        settings.setSupportZoom(true);//设定支持缩放,只设置此属性并不能实现缩放，在某些版本的手机上也只是能实现点击缩放


        web_news.setWebViewClient(new WebViewClient(){
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;
            }
        });

        web_news.setWebViewClient(new WebViewClient(){
            @Override
            public void  onPageStarted(WebView view, String url, Bitmap favicon) {
                //设定加载开始的操作
            }
        });
//        web_news.loadUrl("file:///android_asset/robot.html"); // 页面创建时载入网页
        web_news.loadUrl("http://www.hzau.edu.cn/");
        tab_web = findViewById(R.id.tab_web);
        //tab_tablayout =  (TabLayout) findViewById(R.id.tab_tablayout);
        tab_web.setOnTabSelectedListener(new TabLayout.OnTabSelectedListener() {
            @Override
            public void onTabSelected(TabLayout.Tab tab) {

                //  tab.getPosition()  返回数字，从0开始
                // tab.getText()  返回字符串类型，从0开始
//                if (tab.getPosition()==0){
//                    //sv_eventdetail.setVisibility(View.VISIBLE);//这个是用来显示隐藏的布局用的
//                }

                switch (tab.getPosition())
                {
                    case 0:
                        web_news.loadUrl("http://www.hzau.edu.cn/");
                        break;
                    case 1:
                        web_news.loadUrl("http://www.moa.gov.cn/");
                        break;
                    case 2:
                        web_news.loadUrl("http://www.agri.cn/");
                        break;
                    case 3:
                        web_news.loadUrl("http://agri.china.com.cn/");
                        break;
                    case 4:
                        web_news.loadUrl("http://www.bing.com");
                        break;
                    case 5:
                        web_news.loadUrl("file:///android_asset/robot.html");

                        break;


                        //https://ditu.baidu.com/@12732179.550621318,3543932.263703283,18.89z

                    case 6:
                        break;
                }




            }
            @Override
            public void onTabUnselected(TabLayout.Tab tab) {

            }
            @Override
            public void onTabReselected(TabLayout.Tab tab) {

            }
        });





    }

//    public class NewMap implements Runnable {
//        @Override
//        public void run() {
//
//
//            handler.post(new Runnable() {
//                @Override
//                public void run() {
//                    try {
//                        if (web_news.getProgress() > 50) {
//                            String jsonParams = "116.400244,39.928216";
//                            //String url = "javascript:theLocation()";//不拼接参数，直接调用js的jsMethod函数
//
//                            String script = "javascript:theLocation(" + jsonParams + ")";//拼接参数，就可以把数据传递给j
//                            web_news.loadUrl(script);
//                        }
//                    } catch (Exception e) {
//                        e.printStackTrace();
//                    }
//
//                }
//            });
//
//
//        }
//    }



}