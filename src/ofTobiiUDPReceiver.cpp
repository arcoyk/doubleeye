//
//  ofTobiiUDPReceiver.cpp
//  myUDPReceiver
//
//  Created by Katsuya Fujii on 2/10/16.
//
//

#include "ofTobiiUDPReceiver.h"

ofTobiiUDPReceiver::ofTobiiUDPReceiver(char* receivedData,int port){
    myport=port;
    mystring=receivedData;
    udpConnection.Create();
    udpConnection.Bind(myport);
    cout << "UDP Port: " << myport << " is now open " << endl;
};


void ofTobiiUDPReceiver::reset(){
    
    udpConnection.Close();
    udpConnection.Create();
    udpConnection.Bind(myport);
    udpConnection.SetReceiveBufferSize(61440);
    
};


// the thread function
void ofTobiiUDPReceiver::threadedFunction() {
    
    while(isThreadRunning()) {
        char buff[61440];
        
        int receivedByte=udpConnection.Receive(buff,61440);
        
        if(receivedByte!=-1){
            sprintf(mystring, buff,61440);
            //cout<<mystring<<endl;
        }
        // ofSleepMillis(30);
        
    }
    
};

double  ofTobiiUDPReceiver::get_gaze_x(){
    return ofToDouble(ofSplitString(mystring, ",")[0]);
};

double  ofTobiiUDPReceiver::get_gaze_y(){
    return ofToDouble(ofSplitString(mystring, ",")[1]);
};

double ofTobiiUDPReceiver::get_gaze_timestamp(){
    return ofToDouble(ofSplitString(mystring, ",")[2]);
};