package com.example.whole.ui.home;

import android.content.Intent;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;
import android.widget.ViewFlipper;

import androidx.annotation.Nullable;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProviders;

import com.example.whole.R;
import com.example.whole.news;
import com.example.whole.video;
import com.example.whole.process_img;

public class HomeFragment extends Fragment {
    Button button1,button_process_img,btn_news;
    private HomeViewModel homeViewModel;
    ViewFlipper mFlipper;
    private GestureDetector mDetector;
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        homeViewModel =
                ViewModelProviders.of(this).get(HomeViewModel.class);
        View root = inflater.inflate(R.layout.fragment_home, container, false);
//        final TextView textView = root.findViewById(R.id.text_home);
        button1 = root.findViewById(R.id.show_video); // 界面一创建就连接按钮
        button_process_img = root.findViewById(R.id.process_img);
        btn_news = root.findViewById(R.id.btn_news);
        mFlipper = root.findViewById(R.id.flipper);
        mFlipper.startFlipping();
        mFlipper.setOnTouchListener(this::onTouch);

        mDetector = new GestureDetector(new simpleGestureListener());

        return root;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(), video.class);
                startActivity(intent);
            }
        });
        button_process_img.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(),process_img.class);
                startActivity(intent);
            }
        });

        btn_news.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getActivity(), news.class);
                startActivity(intent);
            }
        });

    }
    public boolean onTouch(View v, MotionEvent event) {
        return mDetector.onTouchEvent(event);
    }

    private class simpleGestureListener extends GestureDetector.SimpleOnGestureListener{
        final int FLING_MIN_DISTANCE = 100, FLING_MIN_VELOCITY = 200;

        //不知道为什么，不加上onDown函数的话，onFling就不会响应，真是奇怪
        @Override
        public boolean onDown(MotionEvent e) {
            // TODO Auto-generated method stub
            //Toast.makeText(MainActivity.this, "ondown", Toast.LENGTH_SHORT).show();
            return true;
        }
        @Override
        public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
                               float velocityY) {
            // Fling left
            if (e1.getX() - e2.getX() > FLING_MIN_DISTANCE
                    && Math.abs(velocityX) > FLING_MIN_VELOCITY) {

                mFlipper.showNext();

                //Toast.makeText(MainActivity.this, "Fling Left", Toast.LENGTH_SHORT).show();
            } else if (e2.getX() - e1.getX() > FLING_MIN_DISTANCE
                    && Math.abs(velocityX) > FLING_MIN_VELOCITY) {
                // Fling right

                mFlipper.showPrevious();

                //Toast.makeText(MainActivity.this, "Fling Right", Toast.LENGTH_SHORT).show();
            }
            return true;
        }
    }


}