//
//  ofThreadReceiver.cpp
//  Global_NY
//
//  Created by Katsuya Fujii on 5/22/15.
//
//

#include "ofThreadReceiver.h"


ofThreadReceiver::ofThreadReceiver(){
    
};


ofThreadReceiver::ofThreadReceiver(char* receivedData,int port){

    myport=port;
    mystring=receivedData;
    udpConnection.Create();
    udpConnection.Bind(myport);
  //  udpConnection.SetNonBlocking(true);
    cout << "UDP Port: " << myport << " is now open " << endl;
};


void ofThreadReceiver::reset(){

    udpConnection.Close();
    
    
    udpConnection.Create();
    udpConnection.Bind(myport);
    udpConnection.SetReceiveBufferSize(receiveSize+2);
  //  udpConnection.SetNonBlocking(true);
    
    
};

// the thread function
void ofThreadReceiver::threadedFunction() {
    
    // start
        
    while(isThreadRunning()) {
       // cout<<"Thread is running" <<endl;
        char buff[130000];
        // int divImageNum = receivedFrame.getWidth()*receivedFrame.getHeight() / receiveSize;
        
     // receivedByte=udpConnection.Receive(buff,130000);
     //   std::cout << "UDP Received " <<byte << std::endl;
        if(receivedByte!=-1){
           // sprintf(mystring, buff,130000);
           // cout<<mystring<<endl;
           }
       // ofSleepMillis(30);
        
    }
    
    // done
};
