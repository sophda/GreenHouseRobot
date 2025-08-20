package com.example.whole.ui.dashboard;

import android.content.Intent;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ViewFlipper;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;

import com.example.whole.Camera;
import com.example.whole.Monitor;
import com.example.whole.NFCActivity;
import com.example.whole.QrCode;
import com.example.whole.R;
import com.example.whole.ReadTextActivity;
import com.example.whole.unity;


public class DashboardFragment extends Fragment {

    private DashboardViewModel dashboardViewModel;
    public Button nfc_button,cam_button,btn_monitor,btn_unity;
    public ImageButton btn_img;



    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        dashboardViewModel =
                ViewModelProviders.of(this).get(DashboardViewModel.class);
        View root = inflater.inflate(R.layout.fragment_dashboard, container, false);
        nfc_button = root.findViewById(R.id.connectNFC);
        cam_button = root.findViewById(R.id.connectCAM);
        btn_monitor = root.findViewById(R.id.btn_monitor);
        btn_img = root.findViewById(R.id.btn_img);
        btn_unity = root.findViewById(R.id.btn_unity);


        btn_monitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(), Monitor.class);
                startActivity(intent);
            }
        });
        btn_img.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(), QrCode.class);
                startActivity(intent);
            }
        });

        nfc_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(), ReadTextActivity.class);
                startActivity(intent);



            }
        });
        cam_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(), Camera.class);
                startActivity(intent);

            }
        });


        btn_unity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(getActivity(), unity.class));
            }
        });


        return root;
    }








}