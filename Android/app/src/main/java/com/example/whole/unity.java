package com.example.whole;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.bottomsheet.BottomSheetDialog;
import com.unity3d.player.UnityPlayerActivity;

import java.io.IOException;

public class unity extends UnityPlayerActivity {

    public TextView txt_zhugao,txt_yemianji, txt_jingcu, txt_yezhouchang,
            txt_jingdu, txt_weidu, txt_id;
    private static Handler handler = new Handler();
    public String content, phenotype,location;

    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);

        setContentView(R.layout.activity_unity);
        LinearLayout ll_unity_container = findViewById(R.id.ll_unity_container);
        View unity_view = mUnityPlayer.getView();
        ll_unity_container.addView(unity_view);
    }

    public void click(View view){
        callMainActivity();
    }

    @Override
    public boolean onKeyDown(int i, KeyEvent keyEvent) {
        // 添加返回键返回 MainActivity
        if(i == KeyEvent.KEYCODE_BACK){
            callMainActivity();
        }
        return super.onKeyDown(i, keyEvent);
    }

    private void callMainActivity(){
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
        finish();

    }

    public String getName(final String str) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
//                Toast.makeText(ret.this, str, ).show();
//                Toast.makeText(unity.this,str,Toast.LENGTH_SHORT).show();
                switch (str)
                {
                    case "zuo":
                        content = "3";showwhole_dialog("");
                        break;
                    case "zhong":
                        content = "2";showwhole_dialog("");
                        break;
                    case "you":
                        content = "1";showwhole_dialog("");
                        break;
                    default:
                        content = "9";
                        break;

                }

            }
        });
        return "done";
    }



    private void showwhole_dialog(String diag_id) {
        BottomSheetDialog dialog=new BottomSheetDialog(this);
        View dialogView= LayoutInflater.from(this).inflate(R.layout.dialog_unity,null);

        txt_zhugao = dialogView.findViewById(R.id.utxt_zhugao);
        txt_yemianji = dialogView.findViewById(R.id.utxt_yemianji);
        txt_jingcu = dialogView.findViewById(R.id.utxt_jingcu);
        txt_yezhouchang = dialogView.findViewById(R.id.utxt_yezhouchang);
        txt_id = dialogView.findViewById(R.id.utxt_id);



        new Thread(new unity.GetThread()).start();

        txt_id.setText(content+"号苗盘");
        dialog.setCanceledOnTouchOutside(true);

        dialog.setContentView(dialogView);
        dialog.show();
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
//                        Toast.makeText(unity.this, DATA[0], Toast.LENGTH_SHORT).show();
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


}