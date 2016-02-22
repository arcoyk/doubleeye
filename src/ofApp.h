#pragma once

#include "ofMain.h"
#include "ofTobiiUDPReceiver.h"

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
    
    struct xyword {
        string word;
        float x;
        float y;
        float timestamp;
        int page;
        int index;
    };
        bool in_word(int x, int y, xyword xyword);
        void render_xyword(vector<xyword> xywords, int crr_page);

    
    xyword xyword_tmp;
    ofVec2f anch, margin;
    ofTrueTypeFont font;
    vector<int> sections;
    vector<xyword> xywords;
    bool render_flag = true;
    int head, tail, crr_page, line_height, font_size;

    // For Tobi Receiver
    ofTobiiUDPReceiver* receiver;
    char receiveData[1000];
};
