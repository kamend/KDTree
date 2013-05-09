#include "testApp.h"

// Based on a OpenProcessing Sketch by Thomas Diewald - http://www.openprocessing.org/sketch/92253

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(30);
	ofSetCircleResolution(120);
	ofEnableAlphaBlending();
	
	ofSeedRandom(10);
	
	int numPoints = 1000;
	
	for(int i=0;i<numPoints;i++) {
		ofVec2f v;
		v = ofVec2f(ofRandom(40, ofGetWidth()-40), ofRandom(40, ofGetHeight()-40));
		points.push_back(v);
	}

	tree = new KDTree(points);

}



//--------------------------------------------------------------
void testApp::update(){

}



//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	
	ofVec2f mPos = ofVec2f(ofGetMouseX(), ofGetMouseY());
	
	NN* nearSearch = new NN(mPos);
	nearSearch->getNN(nearSearch, tree->root);
	
	tree->drawGrid();
	
	// draw result
	ofNoFill();
	float radius = mPos.distance(nearSearch->pnt_nn);
	ofFill();
	ofSetColor(255, 255, 255,100);
	ofCircle(mPos, radius);
	ofSetColor(255,255,255,200);
	ofCircle(mPos, 4);
	ofLine(mPos, nearSearch->pnt_nn);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}