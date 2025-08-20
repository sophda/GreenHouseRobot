package com.example.whole;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;


//TODO : Traditional image process using opencv

public class process_img extends AppCompatActivity {
public Button btn_takephoto,btn_canny, btn_detect, btn_segmentation ;
public Bitmap global_bitmap;
public ImageView imageView;
    static {//加载so库
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_process_img);
        btn_takephoto = findViewById(R.id.btn_takephoto);
        btn_segmentation = findViewById(R.id.btn_segmentation);
        imageView = findViewById(R.id.imageView2);




        btn_takephoto.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                startActivityForResult(cameraIntent,001);
            }
        });
        btn_segmentation.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Bitmap bitmap = global_bitmap.copy(Bitmap.Config.ARGB_8888, true);
                watershed(global_bitmap);
                imageView.setImageBitmap(global_bitmap);

            }
        });

    }
    public void onActivityResult(int requestCode,int resultCode,Intent data)
    {
        Bitmap bitmap = (Bitmap) data.getExtras().get("data");
        global_bitmap = bitmap;
//        watershed(bitmap);
        //getEdge(bitmap);
        imageView.setImageBitmap(bitmap);
    }


    //获得Canny边缘
    native void getEdge(Object bitmap);

    //灰度
    native void gray(Object bitmap);

    //寻找轮廓
    native void contours(Object bitmap);

    // watershed
    native void watershed(Object bitmap);



}
