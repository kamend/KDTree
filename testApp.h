#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "kdtree.h"

// Based on a OpenProcessing Sketch by Thomas Diewald - http://www.openprocessing.org/sketch/92253

using namespace algo;

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	PointList points;
	KDTree *tree;
};
