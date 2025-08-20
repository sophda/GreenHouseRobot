package com.example.whole;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.bottomsheet.BottomSheetDialog;
import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.yzq.zxinglibrary.android.CaptureActivity;
import com.yzq.zxinglibrary.common.Constant;



public class QrCode extends AppCompatActivity {

    public String content, phenotype,location;
    public TextView txt_zhugao,txt_yemianji, txt_jingcu, txt_yezhouchang,
    txt_jingdu, txt_weidu;
    private static Handler handler = new Handler();
    public FloatingActionButton btn_float;
    private WebView webmiaopan;



    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_qr_code);
        Intent intent = new Intent(QrCode.this, CaptureActivity.class);
                startActivityForResult(intent, 0);
                onActivityResult(5,5,intent );
        txt_zhugao = findViewById(R.id.txt_zhugao);
        txt_yemianji = findViewById(R.id.txt_yemianji);
        txt_jingcu = findViewById(R.id.txt_jingcu);
        txt_yezhouchang = findViewById(R.id.txt_yezhouchang);
        btn_float = findViewById(R.id.btn_float);
        txt_jingdu = findViewById(R.id.txt_jingdu);
        txt_weidu = findViewById(R.id.txt_weidu);
        webmiaopan = findViewById(R.id.web_miaopan);


        btn_float.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               showwhole_dialog("");
            }
        });



    }



    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        TextView tvx = findViewById(R.id.txt_id);

        if (requestCode == 0 && resultCode == RESULT_OK) {
            if (data != null) {

                content = data.getStringExtra(Constant.CODED_CONTENT);
                tvx.setText(content+"号苗床");


                webmiaopan.getSettings().setJavaScriptEnabled(true);
                WebSettings settings_ = webmiaopan.getSettings();
//        web_news.getSettings().setDomStorageEnabled(true);
                //settings.setUserAgentString("app/XXX");
//        settings.setJavaScriptEnabled(true);
                settings_.setUseWideViewPort(true);//设定支持viewport属性,有些页面是很早期的只适配了pc的浏览器的页面，页面很大，需要宽大的视野来展示，这时候设置为true就可以在足够的空间拖动，并且页面可以以任意比例缩放。
                settings_.setLoadWithOverviewMode(true);   //自适应屏幕,用尽可能大的视野展示页面，一般和上一属性搭配使用。当然大视野显示的字都很小。
                settings_.setBuiltInZoomControls(true); // 使用缩放控件
                settings_.setDisplayZoomControls(false); // 隐藏缩放控件
                settings_.setSupportZoom(true);

                webmiaopan.loadUrl("http://110.40.172.86:8080/pic/"+content+".jpg");


                new Thread(new GetThread()).start();


            }
        }
    }

    public class GetThread implements Runnable
    {
        @Override
        public void run()
        {

            phenotype = WebService.executeHttpGet("hzau",content);

            handler.post(new Runnable() {
                @Override
                public void run() {
                    try {

                        String DATA[] = phenotype.split("&");
                        Toast.makeText(QrCode.this, DATA[0], Toast.LENGTH_SHORT).show();
                        String phenoarray[] = DATA[0].split(",");
                        location = DATA[1];
//                        location = "114.373065,30.483547";
                        String LOCATION[] = location.split(",");
                        txt_zhugao.setText("株高:"+phenoarray[0]+" mm");
                        txt_yemianji.setText("叶面积:"+phenoarray[1]+" cm^2");
                        txt_jingcu.setText("茎粗:"+phenoarray[2]+" mm");
                        txt_yezhouchang.setText("叶周长:"+phenoarray[3]+" mm");
                        txt_jingdu.setText("经度："+LOCATION[0]);
                        txt_weidu.setText("纬度："+LOCATION[1]);
//                        String phenoarray[] = phenotype.split("|||");
//                        location = "114.373065,30.483547";
//                        String LOCATION[] = location.split(",");
//                        txt_zhugao.setText("株高:"+phenoarray[0]+" mm");
//                        txt_yemianji.setText("叶面积:"+phenoarray[1]+" cm^2");
//                        txt_jingcu.setText("茎粗:"+phenoarray[2]+" mm");
//                        txt_yezhouchang.setText("叶周长:"+phenoarray[3]+" mm");
//                        txt_jingdu.setText("经度："+LOCATION[0]);
//                        txt_weidu.setText("纬度："+LOCATION[1]);

                    }
                    catch (Exception e)
                    {
                        e.printStackTrace();
                    }

                }
            });


        }


    }




    private void showwhole_dialog(String diag_id) {
        BottomSheetDialog dialog=new BottomSheetDialog(this);
        View dialogView=LayoutInflater.from(this).inflate(R.layout.dialog_map,null);

        WebView webView = dialogView.findViewById(R.id.dialog_webmap);

        webView.getSettings().setJavaScriptEnabled(true);
        WebSettings settings = webView.getSettings();
//        web_news.getSettings().setDomStorageEnabled(true);
        //settings.setUserAgentString("app/XXX");
//        settings.setJavaScriptEnabled(true);
        settings.setUseWideViewPort(true);//设定支持viewport属性,有些页面是很早期的只适配了pc的浏览器的页面，页面很大，需要宽大的视野来展示，这时候设置为true就可以在足够的空间拖动，并且页面可以以任意比例缩放。
        settings.setLoadWithOverviewMode(true);   //自适应屏幕,用尽可能大的视野展示页面，一般和上一属性搭配使用。当然大视野显示的字都很小。
        settings.setBuiltInZoomControls(true); // 使用缩放控件
        settings.setDisplayZoomControls(false); // 隐藏缩放控件
        settings.setSupportZoom(true);//设定支持缩放,只设置此属性并不能实现缩放，在某些版本的手机上也只是能实现点击缩放


        webView.setWebViewClient(new WebViewClient(){
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                view.loadUrl(url);
                return true;
            }
        });

        webView.setWebViewClient(new WebViewClient(){
            @Override
            public void  onPageStarted(WebView view, String url, Bitmap favicon) {
                //设定加载开始的操作
            }
        });
        webView.setWebViewClient(new WebViewClient(){
            @Override
            public void  onPageFinished(WebView webView, String url) {

                //String url = "javascript:theLocation()";//不拼接参数，直接调用js的jsMethod函数

                String script = "javascript:theLocation(" + location + ")";//拼接参数，就可以把数据传递给j
                webView.loadUrl(script);
                //设定加载开始的操作
            }
        });


        webView.loadUrl("file:///android_asset/robot.html");


        dialog.setCanceledOnTouchOutside(false);

        dialog.setContentView(dialogView);
        dialog.show();
    }







}
