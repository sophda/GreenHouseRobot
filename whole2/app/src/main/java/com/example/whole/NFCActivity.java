package com.example.whole;

import androidx.appcompat.app.AppCompatActivity;

import android.app.PendingIntent;
import android.content.Intent;
import android.nfc.NfcAdapter;
import android.os.Bundle;
import android.widget.Toast;

public class NFCActivity extends AppCompatActivity {

    private NfcAdapter mNfcAdapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_nfc);

        //初始化nfc
        mNfcAdapter = NfcAdapter.getDefaultAdapter(this);
        PendingIntent mPendingIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);
        if (mNfcAdapter == null) {
            Toast.makeText(NFCActivity.this, "nfc is not available", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
    }


    public void initData() {
        //nfc初始化设置
        NfcUtils nfcUtils = new NfcUtils(this);
    }
    @Override
    protected void onResume() {
        super.onResume();
        //开启前台调度系统
        NfcUtils.mNfcAdapter.enableForegroundDispatch(this, NfcUtils.mPendingIntent, NfcUtils.mIntentFilter, NfcUtils.mTechList);
    }
    @Override
    protected void onPause() {
        super.onPause();
        //关闭前台调度系统
        NfcUtils.mNfcAdapter.disableForegroundDispatch(this);
    }
    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        //当该Activity接收到NFC标签时，运行该方法
        //调用工具方法，读取NFC数据
        try{String str = NfcUtils.readNFCFromTag(intent);}
        catch (Exception e)
        {
            Toast.makeText(NFCActivity.this,"木有NFC",Toast.LENGTH_SHORT).show();
        }



    }
}
