//
//  ofTobiiUDPReceiver.hpp
//  myUDPReceiver
//
//  Created by Katsuya Fujii on 2/10/16.
//
//

#ifndef ofTobiiUDPReceiver_h
#define ofTobiiUDPReceiver_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxNetwork.h"


class ofTobiiUDPReceiver : public  ofThread{
public:
    ofTobiiUDPReceiver(char* receivedData,int port);
    void reset();
    void threadedFunction();
    ofxUDPManager udpConnection;
    
    
    
    int myport;
    char* mystring;
    
    double get_gaze_x();
    double get_gaze_y();
    double get_gaze_timestamp();
    
    
};
#endif /* defined(__Global_NY__ofThreadReceiver__) */
