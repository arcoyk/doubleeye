//
//  ofThreadReceiver.h
//  Global_NY
//
//  Created by Katsuya Fujii on 5/22/15.
//
//

#ifndef __Global_NY__ofThreadReceiver__
#define __Global_NY__ofThreadReceiver__

#include <stdio.h>
#include "ofMain.h"
#include "ofxNetwork.h"


class ofThreadReceiver : public ofThread {
public:
    ofThreadReceiver();
    ofThreadReceiver(char* receivedData,int port);
    void reset();
    void threadedFunction();
    ofxUDPManager udpConnection;
    
    

    int myport;
    char* mystring;
    int receiveSize = 61440;    //60KB(61440byte)
    int receivedByte;
    
};
#endif /* defined(__Global_NY__ofThreadReceiver__) */
