#include "ofApp.h"
#include "fstream"
#include "iostream"
#include "boost/algorithm/string.hpp"


void ofApp::setup(){
    ofBackground(0, 0, 0);
    margin.set(200, 400);
    font_size = 20;
    line_height = font_size * 4;
    anch.set(margin.x, margin.y);
    ofCircle(100, 100, 100);
    ofSetBackgroundAuto(false);
    //font.load("../../liberation.ttf", font_size);
    // font.load("../../vera.ttf", font_size);
    font.load("../../lekton.ttf", font_size);
    //read_xywords();
    
    
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
    // preprocess: convert strwords into xywords with x, y
    for(int i = 0; i < strwords.size(); i++) {
        xyword xyword_tmp = {
            strwords[i],
            anch.x,
            anch.y
        };
        cout << strwords[i] << endl;
        xywords.push_back(xyword_tmp);
        anch.x += (font_size * 3 / 4) * (strwords[i].length() + 1);
        // anch.x += word_offset.x;
        // anch.x += font_size;
        if (anch.x > ofGetWidth() - margin.x * 2 || strwords[i] == "\n" ) {
            anch.x = margin.x;
            anch.y += line_height;
        }
        if (anch.y > ofGetHeight() - margin.y) {
            sections.push_back(i);
            anch.set(margin.x, margin.y);
        }
    }
    sections.push_back(strwords.size() - 1);
    sprintf(receiveData,"0,0,0\n");
    receiver=new ofTobiiUDPReceiver(receiveData,8888);
    receiver->startThread();
    
    string filename="EyeTrack_"+ofGetTimestampString()+".txt";
    file.open( ofToDataPath(filename).c_str());
    file << "page,x,y,word,word_index,timestamp" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    double tmp_x = receiver->get_gaze_x();
    double y = receiver->get_gaze_y();
    double time = receiver->get_gaze_timestamp();
    double x=ofMap(tmp_x, -1920,0,0,1920);
    draw_circle(x, y);
    // render_xyword(xywords, crr_page);
    for(int i = head; i <= tail; i++) {
        xyword_tmp = xywords[i];
        ofVec2f word_posi = ofVec2f(xyword_tmp.x, xyword_tmp.y);
        ofVec2f eye_posi = ofVec2f(x, y);
        if (in_word(x, y, xyword_tmp)) {
            ofSetColor(255, 100, 100, 255);
            //font.drawString(xyword_tmp.word, xyword_tmp.x, xyword_tmp.y);

            cout << x << "," << y << "," << time << "," << xyword_tmp.word << "," <<crr_page<<endl;
            file<< crr_page << "," << x << ","  <<y << ","<< index << "," << xyword_tmp.word << "," <<time<<endl;

            break;
        }
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
  
   // ofCircle(x, y, 5);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 0, 0, 100);
    // ofCircle(100, 100, 100);
    if (render_flag) {
        render_page();
        render_eyeprint(eyeprints, crr_page);
        render_flag = false;
    }
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
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  //  eyeprints = readFile("EyeTrack_2016-02-22-21-55-36-190.txt");
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//    for(int i = head; i <= tail; i++) {
//        xyword_tmp = xywords[i];
//        ofVec2f word_posi = ofVec2f(xyword_tmp.x, xyword_tmp.y);
//        ofVec2f mouse_posi = ofVec2f(x, y);
//        if (word_posi.distance(mouse_posi) < 10) {
//            ofSetColor(255, 100, 100, 255);
//            ord, xyword_tmp.x, xyword_tmp.y);
//            break;
//        }
//    }
//    ofSetColor(0, 255, 0, 100);
//    ofCircle(x, y, 5);
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


