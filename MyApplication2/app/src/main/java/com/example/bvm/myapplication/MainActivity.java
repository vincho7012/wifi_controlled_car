package com.example.bvm.myapplication;

import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ToggleButton;


import java.io.*;
import java.net.*;
import java.util.concurrent.TimeUnit;

import static android.R.id.button1;
import static com.example.bvm.myapplication.R.id.button_up;
import static com.example.bvm.myapplication.R.id.textView;

public class MainActivity extends AppCompatActivity implements View.OnTouchListener{

    public MainActivity() throws SocketException {
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button button1 = (Button)findViewById(R.id.button_up);
        Button button2 = (Button)findViewById(R.id.button_down);
        Button button3 = (Button)findViewById(R.id.button_left);
        Button button4 = (Button)findViewById(R.id.button_right);
        Button button5 = (Button)findViewById(R.id.lightsButton);
        button1.setOnTouchListener(this);
        button2.setOnTouchListener(this);
        button3.setOnTouchListener(this);
        button4.setOnTouchListener(this);
        button5.setOnTouchListener(this);

        thread.start();

    }



    /*DatagramSocket serverSocket = new DatagramSocket(4210);
    byte[] receiveData = new byte[8];
    DatagramPacket receivePacket = new DatagramPacket(receiveData,
            receiveData.length);*/


    DatagramSocket clientSocket = new DatagramSocket();
    DatagramPacket packet_to_send;
    boolean formed_package = false;
    boolean lights = false;


    public boolean onTouch(View v,MotionEvent event) {
       // System.out.println("123");
/*
        if(event.getAction() == MotionEvent.ACTION_MOVE)
        {
            try {
                if (v != (Button) findViewById(R.id.lightsButton)) {
                    packet_to_sand = pressed_udp(v);
                    TimeUnit.MILLISECONDS.sleep(2*00);
                }

            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }
 */
        if(event.getAction() == MotionEvent.ACTION_DOWN){
            try {

                pressed_udp(v);

            }catch(Exception e)
            {

            }
        }
        if(event.getAction() == MotionEvent.ACTION_UP)
        {
            try {

                pressed_udp(null);
                TimeUnit.MILLISECONDS.sleep(1000);

            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }

        return true;

    }


    Thread thread = new Thread(new Runnable() {

        @Override
        public void run() {
            try  {
                while(true) {
                    if (formed_package == true) {
                        clientSocket.send(packet_to_send);
                        //clientSocket.send(packet_to_send);
                        formed_package = false;
                    }
                   /* serverSocket.receive(receivePacket);
                    TextView tv = (TextView) findViewById(R.id.textView);
                    tv.setText(String.valueOf(receiveData));*/
                }
                //Your code goes here
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    });





    public void pressed_udp(View view) throws IOException, InterruptedException {

        InetAddress IPAddress = InetAddress.getByName("192.168.4.1");
        byte[] sendData = new byte[2];
        //byte[] receiveData = new byte[1024];
        if (view == (Button) findViewById(R.id.button_up)) {
            sendData[0] = 'F';
            System.out.println("F");
        }
        if (view == (Button) findViewById(R.id.button_down)) {
            sendData[0] = 'B';
            System.out.println("B");
        }
        if (view == (Button) findViewById(R.id.button_left)) {
            sendData[0] = 'L';
            System.out.println("L");
        }
        if (view == (Button) findViewById(R.id.button_right)) {
            sendData[0] = 'R';
            System.out.println("R");
        }
        if(view == (Button) findViewById(R.id.lightsButton))
        {
            if(lights == true) {
                sendData[0] = 'X';
                lights = false;
            }
            else
            {
                sendData[0] = 'W';
                lights = true;
            }

        }
        if(view == null)
        {
            sendData[0] = 'U';
        }
        sendData[1] = sendData[0];
        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, 4210);
       // System.out.println(IPAddress);
        packet_to_send = sendPacket;
        formed_package = true;
       // return sendPacket;


    }


}



