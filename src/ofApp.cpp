#include "ofApp.h"
#include "fstream"
#include "iostream"
#include "boost/algorithm/string.hpp"


void ofApp::setup(){
    ofBackground(0, 0, 0);
    margin.set(100, 100);
    font_size = 20;
    line_height = 50;
    anch.set(margin.x, margin.y);
    ofCircle(100, 100, 100);
    ofSetBackgroundAuto(false);
    font.load("../../VeraMono.ttf", font_size);
    
    // read words into strwords
    ifstream script_stream = ifstream("../../../../script.txt");
    string script;
    string delims = " ";
    vector<string> strwords;
    vector<string> words_tmp;
    while(std::getline(script_stream, script)) {
        boost::split(words_tmp, script, boost::is_any_of(delims),
                     boost::algorithm::token_compress_on);
        copy(words_tmp.begin(), words_tmp.end(), back_inserter(strwords));
    }
    
    // convert strwords into xywords with x, y
    for(int i = 0; i < strwords.size(); i++) {
        boost::trim(strwords[i]);
        xyword xyword_tmp = {
            strwords[i],
            anch.x,
            anch.y
        };
        xywords.push_back(xyword_tmp);
        anch.x += font_size * strwords[i].length();
        // anch.x += word_offset.x;
        // anch.x += font_size;
        if (anch.x > ofGetWidth() - margin.x || strwords[i] == "\n" ) {
            anch.x = margin.x;
            anch.y += line_height;
        }
        if (anch.y > ofGetHeight() - margin.y) {
            sections.push_back(i);
            anch.set(margin.x, margin.y);
        }
    }
    sections.push_back(strwords.size() - 1);
    
    receiver=new ofTobiiUDPReceiver(receiveData,8888);
    receiver->startThread();
}

//--------------------------------------------------------------
void ofApp::update(){
    cout << "x: "<< receiver->get_gaze_x()<<endl;
    cout << "y: "<<receiver->get_gaze_y()<<endl;
    cout << "timestap: "<<receiver->get_gaze_timestamp()<<endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 0, 0, 100);
    // ofCircle(100, 100, 100);
    if (render_flag) {
        render_page();
        render_flag = false;
    }
}

void ofApp::render_page() {
    ofBackground(0, 0, 0);
    ofSetColor(255, 255, 255, 255);
    anch.set(margin.x, margin.y);
    head = crr_page == 0 ? 0 : sections[crr_page - 1] + 1;
    tail = sections[crr_page];
    xyword tmp = {"", 0, 0};
    for(int i = head; i <= tail; i++) {
        tmp = xywords[i];
        // font.drawString(to_string(tmp.word.length()), tmp.x, tmp.y);
        ofCircle(tmp.x, tmp.y, 1);
        ofDrawBitmapString(std::to_string(tmp.word.length()),
                           tmp.x + font_size * tmp.word.length(),
                           tmp.y, 2);
        font.drawString(tmp.word, tmp.x, tmp.y);
    }
    string page_info = std::to_string(crr_page + 1) +
                        "/" +
                        to_string(sections.size());
    ofDrawBitmapString(page_info, 10, 10);
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
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    for(int i = head; i <= tail; i++) {
        xyword_tmp = xywords[i];
        ofVec2f word_posi = ofVec2f(xyword_tmp.x, xyword_tmp.y);
        ofVec2f mouse_posi = ofVec2f(x, y);
        if (word_posi.distance(mouse_posi) < 10) {
            ofSetColor(255, 100, 100, 255);
            font.drawString(xyword_tmp.word, xyword_tmp.x, xyword_tmp.y);
            break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
