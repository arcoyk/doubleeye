#include "ofApp.h"
#include "fstream"
#include "iostream"
#include "boost/algorithm/string.hpp"


#define LOG_FILE_NAME "matsuda.txt"
#define SCRIPT_FILE_NAME "../../../../scripts/utokyo.txt"
// utokyo.txt toritsu.txt center.txt jobs.txt

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
      //  cout << strwords[i] << endl;
        xywords.push_back(xyword_tmp);
        anch.x += (font_size * 3 / 4) * (strwords[i].length() + 1);
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
    // Start UDP
    sprintf(receiveData,"0,0,0\n");
    receiver = new ofTobiiUDPReceiver(receiveData,8888);
    receiver->startThread();
    // Prepare log file
    filename="EyeTrack_"+ofGetTimestampString()+".txt";
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
             ofSetColor(0, 255, 0, 100);
            render_eyeprint(eyeprints, crr_page);
            render_fixed_words(eyeprints, crr_page);
        }
        if(show_master){
             ofSetColor(0, 0, 255, 100);
        
            render_eyeprint(eyeprints2, crr_page);
            ofSetColor(0, 0, 255, 100);
            render_fixed_words(eyeprints2, crr_page);
        }
        render_flag = false;
    }
    // デバッグ表示
    if(debug) {
        // draw_debug(curr_eye.x, curr_eye.y);
        ofSetColor(0, 255, 0, 100);
        draw_circle(curr_eye.x, curr_eye.y);
    }
}

bool ofApp::in_word(int x, int y, xyword xyword) {
    float min_dist = 30.0;
    int length = xyword.word.length() *  font_size * 1 / 2;
    int height = font_size;
    ofVec2f center = ofVec2f(xyword.x + length / 2,
                             xyword.y - height / 2);
    if (ofDist(center.x, center.y, x, y) < min_dist) {
        // ofLine(x, y, center.x, center.y);
        // ofCircle(center.x, center.y, min_dist);
        return true;
    } else {
        return false;
    }
}

void ofApp::draw_circle(int x, int y) {
   
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
    xyword tmp = {"", 0, 0, 0};
    for(int i = head; i <= tail; i++) {
        tmp = xywords[i];
        font.drawString(tmp.word, tmp.x, tmp.y);
    }
    string page_info = std::to_string(crr_page + 1) +
                        "/" +
                        to_string(sections.size());
    ofDrawBitmapString(page_info, 10, 10);
}

void ofApp::render_fixed_words(vector<eyeprint> eyeprints, int crr_page) {
    float min_weight = 10;
    float min_dist = 40.0;
    eyeprint eyeprint;
    xyword xyword;
    for (int i = 0; i < eyeprints.size(); i++) {
        eyeprint = eyeprints[i];
        if (eyeprint.page != crr_page) {
            continue;
        } else {
            // word.weight++ if the eyeprint is near enough
            for (int h = head; h <= tail; h++) {
                xyword = xywords[h];
                if (in_word(eyeprint.x, eyeprint.y, xyword)) {
                    xywords[h].weight++;
                }
            }
        }
    }
    // render words if word.weight is over min_weight
    for (int i = head; i <= tail; i++) {
        xyword = xywords[i];
        if (xyword.weight > min_weight) {
            ofSetColor(xyword.weight * 255, 0, 0);
            font.drawString(xyword.word, xyword.x, xyword.y);
        }
    }
}

void ofApp::render_eyeprint(vector<eyeprint> eyeprints, int crr_page) {
    eyeprint prev = {0, 0, 0, "", 0, 0};
    for(int i = 0; i < eyeprints.size(); i++) {
        eyeprint ep = eyeprints[i];
        if (ep.page == crr_page ) {
            draw_circle(ep.x, ep.y);
            ofLine(prev.x, prev.y, ep.x, ep.y);
        }
        prev.x = ep.x;
        prev.y = ep.y;
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
            debug =! debug;
            break;
            
        case 'r':
            //Recordのon/off
            record_log =! record_log;
            break;
            
        case 'l':
            //Log読み込みのon/ofG
#ifdef LOG_FILE_NAME
             eyeprints = readFile(LOG_FILE_NAME);
#else
            file.close();
             eyeprints = readFile(filename);
#endif
           
            show_log =! show_log;
            break;
            
        case 'm':
            //Recordのon/off
            show_master=!show_master;
           eyeprints2 = readFile("Alicia.txt");
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


