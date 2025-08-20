package com.example.whole;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.FileProvider;

import android.app.Dialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;


import com.example.whole.pytorch.Classifier;
import com.example.whole.pytorch.Utils;
import com.google.android.material.bottomsheet.BottomSheetDialog;


import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.ByteBuffer;


public class Camera extends AppCompatActivity {

    int cameraRequestCode = 1;
    private Button btn_take_photo;
    public Button btn_connect,btn_camera;
    public TextView result;
    public ImageView imageView ;
    public Bitmap global_bitmap=null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera);

        result = findViewById(R.id.textView);
        btn_connect = findViewById(R.id.btn_connect);
        btn_camera = findViewById(R.id.btn_camera);

        imageView = findViewById(R.id.imageView);
        btn_camera.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);

                startActivityForResult(cameraIntent,001);

            }
        });
        btn_connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (global_bitmap != null) {
                    String classify_result = classify(global_bitmap);
                    result.setText(classify_result);

                    //--------独立式dialog--------
//                    showBottomSheetDialog();
//                    showBottomDialog();

                    showwhole_dialog(classify_result);


                }
                else{
                    Toast.makeText(Camera.this,"请拍摄要连接的设备",Toast.LENGTH_SHORT).show();
                }

            }
        });

    }


    private String classify(Bitmap bitmap) {
        Classifier classifier;
        classifier = new Classifier(Utils.assetFilePath(this, "mobilenet-v2.pt"));
        String pred = classifier.predict(bitmap);
        return pred;


    }

    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {

        Bitmap imageBitmap = (Bitmap) data.getExtras().get("data");

        imageView.setImageBitmap(imageBitmap);

        global_bitmap = imageBitmap;
        }


    private void showBottomSheetDialog() {
        final BottomSheetDialog dialog=new BottomSheetDialog(this);
        View dialogView= LayoutInflater.from(this).inflate(R.layout.dialog_myrobot,null);

        dialog.setCanceledOnTouchOutside(false);
        dialog.setContentView(dialogView);
        dialog.show();
    }

    private void showBottomDialog() {
        View view = LayoutInflater.from(this).inflate(R.layout.dialog_myrobot, null);
        Dialog dialog = new Dialog(this);
        Window window = dialog.getWindow();
        window.setGravity(Gravity.BOTTOM);
        window.setContentView(view);
        WindowManager.LayoutParams lp = window.getAttributes();
        lp.width = WindowManager.LayoutParams.MATCH_PARENT;
        lp.height = WindowManager.LayoutParams.WRAP_CONTENT;
        window.setAttributes(lp);
        dialog.show();
    }

    //--------一体式dialog----------------------------
    private void showwhole_dialog(String diag_id) {
        final BottomSheetDialog dialog=new BottomSheetDialog(this);
        View dialogView=null;
        switch (diag_id)
        {

            case "laptop, laptop computer":
                dialogView= LayoutInflater.from(this).inflate(R.layout.dialog_mylaptop,null);
                break;
            default:
                dialogView= LayoutInflater.from(this).inflate(R.layout.dialog_myrobot,null);
                break;
        }

        Button btn_jump = dialogView.findViewById(R.id.btn_diaglog_connect);
        btn_jump.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Camera.this,video.class);
                intent.putExtra("IP","192.168.43.178");
                startActivity(intent);
            }
        });


        dialog.setCanceledOnTouchOutside(false);

        dialog.setContentView(dialogView);
        dialog.show();
    }
}

