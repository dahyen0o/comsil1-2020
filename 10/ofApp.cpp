#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second
    
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    
    draw_flag = 0;
    load_flag = 0;
	fall_flag = 0;
    dot_diameter = 20.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127,23,31);  // Set the drawing color to brown
    
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);
    
    
    ofSetLineWidth(5);
    if( draw_flag ){
        
        
        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
         */
		for (int i = 0;i < num_of_line;i++)
			ofDrawLine(line_arr[i].x1, line_arr[i].y1, line_arr[i].x2, line_arr[i].y2);
		
		ofSetColor(0, 0, 0);
		for (int i = 0;i < num_of_dot;i++) 
			ofDrawCircle(dot_arr[i].x, dot_arr[i].y, 10);

		ofSetColor(255, 0, 0);
		if (draw_flag == 2) {
			int x = dot_arr[dot_index].x;
			int y = dot_arr[dot_index].y;
			ofDrawCircle(x, y, 10);
		}
		else ofDrawCircle(dot_arr[0].x, dot_arr[0].y, 10);
      
        // 2nd week portion.
        ofSetLineWidth(3);
		if (fall_flag) {
			ofSetColor(0, 0, 255);
			for (int i = 0;i < fall_index - 1;i++) {
				ofDrawLine(water_arr[i].x, water_arr[i].y, water_arr[i + 1].x, water_arr[i + 1].y);
			}
		}
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q'){
        // Reset flags
        draw_flag = 0;
        
        // Free the dynamically allocated memory exits.
		delete[] dot_arr;
		delete[] line_arr;
        
        cout << "Dynamically allocated memory has been freed." << endl;
        
        _Exit(0);
    }
    if (key == 'd'){
        if( !load_flag) return;
        
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
		draw_flag = 1;
		draw();
    }
    if (key == 's'){
		if (!load_flag) return;
		if (!draw_flag) return;
		fall_flag = 1;

		water_arr = new DNODE[1000];
		int x[100]; // x 범위를 비교했을 때 해당하는 line의 index 저장
		int i, j, k, flag = 0, k_index = 9999;
		int water_x = dot_arr[dot_index].x;
		int water_y = dot_arr[dot_index].y;
		water_arr[0].x = water_x;
		water_arr[0].y = water_y;
		fall_index = 1;

		while (water_y != 728) {
			j = 0;
			// step 1. 현재 점의 x좌표가 선분의 x좌표 내에 있는지
			for (i = 0;i < num_of_line;i++) {
				int small_x = line_arr[i].x1, big_x = line_arr[i].x1;
				int big_y = line_arr[i].y1;
				if (small_x > line_arr[i].x2) small_x = line_arr[i].x2;
				else big_x = line_arr[i].x2;
				if (big_y < line_arr[i].y2) big_y = line_arr[i].y2;

				if (small_x <= water_x && big_x >= water_x && big_y >= water_y) {
					x[j++] = i;
				}
			} // i: line index
			float y[100]; // y 좌표들 비교할 배열
			if (!j) {
				water_y = 728;
			} // 모든 선분에 닿지 않음

			else {
				// step 2. 해당되는 선분들 중 가장 위에 있는 선분 고르기
				for (k = 0;k < j;k++) {
					y[k] = line_arr[x[k]].y2 - line_arr[x[k]].y1;
					y[k] = y[k] / (line_arr[x[k]].x2 - line_arr[x[k]].x1);
					y[k] = y[k] * (water_x - line_arr[x[k]].x1) + line_arr[x[k]].y1;
				}
				int small_y = 9999;
				for (k = 0;k < j;k++) {
					if (y[k] >= water_y && small_y > (int)y[k]) {
						small_y = (int)y[k];
						k_index = k;
					}
				}
				if (small_y == 9999) water_y = 728;
				else {
					water_y = small_y;
				}
			} // 하나라도 닿는 선분이 있을 때
			
			// step 3. water_arr 에 반영하기
			water_arr[fall_index].x = water_x;
			water_arr[fall_index].y = water_y;
			fall_index++;
			if (water_y != 728) {
				water_arr[fall_index].x = water_x;
				water_arr[fall_index].y = water_y;
				int Big_y = line_arr[x[k_index]].y1;
				int Big_x = line_arr[x[k_index]].x1;
				
				if (Big_y < line_arr[x[k_index]].y2) {
					Big_y = line_arr[x[k_index]].y2;
					Big_x = line_arr[x[k_index]].x2;
				}
				
				water_arr[fall_index].x = Big_x;
				water_arr[fall_index].y = Big_y;
				fall_index++;
				
				water_x = Big_x;
				water_y = Big_y + 1; // 무한 loop 방지
			}
			else break;
		}
        // 2nd week portion.
    }
    if (key == 'e'){
		fall_flag = 0;
		delete[] water_arr;
        // 2nd week portion.
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == 'l'){
        // Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only txt for Waterfall");
        
        // Check whether the user opened a file
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    
    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */
    
    if (key == OF_KEY_RIGHT){
		if (!fall_flag) {
			draw_flag = 2;

			if (dot_index == num_of_dot - 1) dot_index = 0;
			else dot_index++;

			cout << "Selcted Dot Coordinate is (" << dot_arr[dot_index].x << ", " << dot_arr[dot_index].y << ")" << endl;
		}
    }
    if (key == OF_KEY_LEFT){
		if (!fall_flag) {
			draw_flag = 2;

			if (dot_index == 0) dot_index = num_of_dot - 1;
			else dot_index--;

			cout << "Selcted Dot Coordinate is (" << dot_arr[dot_index].x << ", " << dot_arr[dot_index].y << ")" << endl;
		}
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) { 
    //Path to the comma delimited file
    //string fileName = "input.txt";
    
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    if( !file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;
    
    ofBuffer buffer(file);
    
    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line input is in progress.
     IF input_type == 1, then work of getting dot input is in progress.
     */
    int input_type = 0;
	int count = 0;
    
    
    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */
    
    
    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");
        
        if( words.size() == 1){
            if( input_type == 0){ // Input for the number of lines.
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
				
				line_arr = new LNODE[num_of_line];
			}
            else{ // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
			
				dot_arr = new DNODE[num_of_dot];
				count = 0;
			}
        }
        else if (words.size() >= 2){
            int x1,y1,x2,y2;
            if( input_type == 0){ // Input for actual information of lines
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                x2 = atoi(words[2].c_str());
                y2 = atoi(words[3].c_str());

				line_arr[count].y1 = y1;
				line_arr[count].y2 = y2;
				line_arr[count].x1 = x1;
				line_arr[count].x2 = x2;

				count++;
            }
            else{ // Input for actual information of dots.
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());

				dot_arr[count].x = x1;
				dot_arr[count].y = y1;
				count++;
            }
        } // End of else if.

		if (count == num_of_line) input_type = 1;
    } // End of for-loop (Read file line by line).
    
	// dot_arr selection sort
	int min, index = 0;
	for (int i = 0;i < num_of_dot;i++) {
		min = 99999;
		for (int j = i;j < num_of_dot;j++) {
			if (min > dot_arr[j].x) {
				min = dot_arr[j].x;
				index = j;
			}
		}
		int t1 = dot_arr[i].x;
		dot_arr[i].x = dot_arr[index].x;
		dot_arr[index].x = t1;

		int t2 = dot_arr[i].y;
		dot_arr[i].y = dot_arr[index].y;
		dot_arr[index].y = t2;
	}


    initializeWaterLines();
}

void ofApp::initializeWaterLines() {
	fall_index = 0;
}


