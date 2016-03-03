#include "ofApp.h"
#include "fstream"
#include "iostream"
#include "boost/algorithm/string.hpp"

#define LOG_FILE_NAME "EyeTrack_2016-02-22-21-55-36-190.txt"
#define SCRIPT_FILE_NAME "script.txt"

void ofApp::setup(){
    ofBackground(0, 0, 0);
    margin.set(200, 400);
    font_size = 20;
    line_height = font_size * 4;
    anch.set(margin.x, margin.y);
    ofCircle(100, 100, 100);
    ofSetBackgroundAuto(false);
    // Set font here: lekton.ttf or vera.ttf or liberation.tff
    font.load("../../lekton.ttf", font_size);
    ifstream script_stream = ifstream(SCRIPT_FILE_NAME);
    string script;
    string delims = " ";
    vector<string> strwords;
    vector<string> words_tmp;
    while(std::getline(script_stream, script)) {
        boost::split(words_tmp, script, boost::is_any_of(delims),
                     boost::algorithm::token_compress_on);
        copy(words_tmp.begin(), words_tmp.end(), back_inserter(strwords));
    }
    for(int i = 0; i < strwords.size(); i++) {
        ofApp::xyword xyword_tmp = {
            strwords[i],
            anch.x,
            anch.y
        };
        cout << strwords[i] << endl;
        xywords.push_back(xyword_tmp);
        anch.x += (ofApp::font_size * 3 / 4) * (strwords[i].length() + 1);
        // anch.x += word_offset.x;
        // anch.x += font_size;
        if (anch.x > ofGetWidth() - ofApp::margin.x * 2 || strwords[i] == "\n" ) {
            anch.x = margin.x;
            anch.y += line_height;
        }
        if (anch.y > ofGetHeight() - margin.y) {
            sections.push_back(i);
            anch.set(margin.x, margin.y);
        }
    }
    sections.push_back(strwords.size() - 1);
    // script_to_xywords();
    // Start UDP
    sprintf(receiveData,"0,0,0\n");
    receiver = new ofTobiiUDPReceiver(receiveData,8888);
    receiver->startThread();
    // Prepare log file
    string filename="EyeTrack_"+ofGetTimestampString()+".txt";
    file.open( ofToDataPath(filename).c_str());
    file << "page,x,y,word,word_index,timestamp" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    // Tobiiから視線情報を取得
    double tmp_x = receiver->get_gaze_x();
    curr_eye.x = ofMap(tmp_x, -1920,0,0,1920);
    curr_eye.y = receiver->get_gaze_y();
    double time = receiver->get_gaze_timestamp();
    file << crr_page << "," << curr_eye.x << ","  << curr_eye.y << ","<< index << "," << xyword_tmp.word << "," << time << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 0, 0, 100);
    // ofCircle(100, 100, 100);
    if (render_flag) {
        render_page();
        if (show_log) {
            render_eyeprint(eyeprints, crr_page);
        }
        // デバッグ表示
        if(debug) {
            draw_debug(curr_eye.x, curr_eye.y);
            draw_circle(curr_eye.x, curr_eye.y);
        }
        
        render_flag = false;
    }
}

bool ofApp::in_word(int x, int y, xyword xyword) {
    int word_length = xyword.word.length() *  font_size * 1 / 2;
    int word_height = font_size;
    if (xyword.x < x &&
        x < xyword.x + word_length &&
        xyword.y < y &&
        y < xyword.y + word_height) {
        return true;
    }
    return false;
}

void ofApp::draw_circle(int x, int y) {
    ofSetColor(0, 255, 0, 100);
    ofCircle(x, y, 5);
}

void ofApp::draw_debug(double x, double y) {
    ofSetColor(0,0,0);
    ofDrawBitmapString("X: "+ofToString(x)+ ", Y: "+ofToString(y), 10,50);
}

void ofApp::render_page() {
    ofBackground(255);
    ofSetColor(0);
    anch.set(margin.x, margin.y);
    head = crr_page == 0 ? 0 : sections[crr_page - 1] + 1;
    tail = sections[crr_page];
    xyword tmp = {"", 0, 0};
    for(int i = head; i <= tail; i++) {
        tmp = xywords[i];
        font.drawString(tmp.word, tmp.x, tmp.y);
    }
    string page_info = std::to_string(crr_page + 1) +
                        "/" +
                        to_string(sections.size());
    ofDrawBitmapString(page_info, 10, 10);
}

void ofApp::render_eyeprint(vector<eyeprint> eyeprints, int crr_page) {
    for(int i = 0; i < eyeprints.size(); i++) {
        eyeprint ep = eyeprints[i];
        if (ep.page == crr_page ) {
            draw_circle(ep.x, ep.y);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 356) {
        // prev page
        if (crr_page != 0) {
            crr_page--;
        }
    } else if (key == 358) {
        // next page
        if (crr_page < sections.size() - 1) {
            crr_page++;
        }
    }
    render_flag = true;
    
    switch (key){
        case 'f':
            //フルスクリーンのon/off
            ofToggleFullscreen();
            
        case 'g':
            //Gui表示のon/off
//            gui.toggleDraw();
              break;
        case 'd':
            //Debug表示のon/off
            debug=!debug;
            break;
            
        case 'r':
            //Recordのon/off
            record_log=!record_log;
            break;
            
        case 'l':
            //Log読み込みのon/off
            eyeprints = readFile(LOG_FILE_NAME);
            show_log =! show_log;
            break;
    }
}


void ofApp::keyReleased(int key){};
void ofApp::mouseMoved(int x, int y ){};
void ofApp::mouseDragged(int x, int y, int button){};
void ofApp::mousePressed(int x, int y, int button){};
void ofApp::mouseReleased(int x, int y, int button){};
void ofApp::mouseEntered(int x, int y){};
void ofApp::mouseExited(int x, int y){};
void ofApp::windowResized(int w, int h){};
void ofApp::dragEvent(ofDragInfo dragInfo){};
void ofApp::gotMessage(ofMessage msg){};

void script_to_xywords() {

}

vector<ofApp::eyeprint> ofApp::readFile(string filename){
    ifstream file;
    file.open( ofToDataPath(filename).c_str() );
    vector<eyeprint> eyeprints;
    int count=0;
    while(!file.eof())
    {
//        cout << "Read lines " << count << endl;
        if(count){ //skip first line
            string str;
            getline(file, str);
            vector<string> line=ofSplitString(str, ",");
            if(line.size()>2){
                eyeprint tmp;
                tmp.page=ofToInt(line[0]);
                tmp.x=ofToFloat(line[1]);
                tmp.y=ofToFloat(line[2]);
                tmp.word=ofToFloat(line[3]);
                tmp.word_index=ofToInt(line[4]);
                tmp.timestamp=ofToFloat(line[5]);
                eyeprints.push_back(tmp);
            }
        }
        count++;
    }
    
    // cout << "Read lines " << count << endl;
    
    file.close();
    return eyeprints;
}


