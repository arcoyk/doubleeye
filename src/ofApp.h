#pragma once

#include "ofMain.h"
#include "ofTobiiUDPReceiver.h"
#include <fstream>  

class ofApp : public ofBaseApp{
    public:
    void setup();
    void update();
    void draw();
    void render_page();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void draw_circle(int x, int y);
    void draw_debug(double x, double y);
    void script_to_xywords();
    
    struct xyword {
        string word;
        float x;
        float y;
        float weight;
    };
    
    struct eyeprint {
        int page;
        float x;
        float y;
        string word;
        int word_index;
        float timestamp;
    };
    
    vector<eyeprint> readFile(string filename);
    string filename;
    
    ofstream file;

    bool in_word(int x, int y, xyword xyword);
    void render_eyeprint(vector<eyeprint> eyeprints, int crr_page);
    void render_fixed_words(vector<eyeprint> eyeprints, int crr_page);

    xyword xyword_tmp;
    ofVec2f anch, margin;
    ofTrueTypeFont font;
    ofVec2f curr_eye;
    vector<eyeprint> eyeprints;
     vector<eyeprint> eyeprints2;
    vector<int> sections;
    vector<xyword> xywords;
    bool render_flag = true;
    
    bool debug=false, show_log=false, record_log=false,show_master=false;
    
    int head, tail, crr_page, line_height, font_size,index;
    float run_from_time;
    

    // For Tobi Receiver
    ofTobiiUDPReceiver* receiver;
    char receiveData[1000];
};
