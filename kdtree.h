//
//  kdtree.h
//  ofxCvTemplate
//
//  Created by Kamen Dimitrov on 5/8/13.
//
//

// Based on a OpenProcessing Sketch by Thomas Diewald - http://www.openprocessing.org/sketch/92253

#ifndef ofxCvTemplate_kdtree_h
#define ofxCvTemplate_kdtree_h

#include "ofMain.h"

namespace algo {

typedef vector<ofVec2f> PointList;
typedef PointList::iterator PointListItr;

class KDNode {
public:
	int depth;
	ofVec2f pnt;
	KDNode *l, *r;
	
	KDNode(int _depth) : depth(_depth) { };
	bool isLeaf() { return ((l == NULL) | (r == NULL)); }
};

struct compareY {
	bool operator()(const ofVec2f &pnt1, const ofVec2f &pnt2) {
		return pnt1.y < pnt2.y;
	}
};
		
struct compareX {
	bool operator()(const ofVec2f &pnt1,const ofVec2f &pnt2) {
		return pnt1.x < pnt2.x;
	}
};

class KDTree {
public:
	int max_depth = 0;
	KDNode *root;
	
	KDTree(PointList list) {
		
		PointList::size_type numPoints = list.size();
		
		max_depth = (int) ceil(log(numPoints)/log(2));
		root = new KDNode(0);
		build(root,list);
	}
	
	// we work with the copy of PointList everytime
	// because during sorting, if we sort the original
	// this will mess things up
	// !!! some optimization is needed here !!!
	
	void build(KDNode *node,PointList list) {
		node->l=node->r=NULL;
		
		PointList::size_type e = list.size();
		if(e == 0) return;
		int m = e >> 1;
		if( e > 1) {
			int depth = node->depth;
	
			if((depth & 1)) { // odd Y
				std::sort(list.begin(), list.end(), compareY());
			} else { // even X
				std::sort(list.begin(), list.end(), compareX());
			}
			
			node->l = new KDNode(depth+1);
			node->r = new KDNode(depth+1);
			
			PointListItr begin = list.begin();
			PointListItr median = begin+m;
			PointListItr end = list.end();
			
			build (node->l, PointList(begin, median));
			build (node->r, PointList(median, end));
		}		
		node->pnt = *(list.begin()+m);
	}
	
	int numLeafs(KDNode* node, int num_leafs) {
		if(node->isLeaf()) {
		
			return num_leafs+1;
		} else {
			num_leafs = numLeafs(node->l, num_leafs);
			num_leafs = numLeafs(node->r, num_leafs);
			return num_leafs;
		}
	}
	
	int numLeafs() {
		return numLeafs(root, 0);
	}
	
	void drawPlanes(KDNode *node, float x_min, float y_min, float x_max, float y_max) {
		
		
		if(node != NULL) {
			ofVec2f pnt = node->pnt;
			
			if((node->depth & 1) == 0) {
				
				drawPlanes(node->l, x_min, y_min, pnt.x, y_max);
				drawPlanes(node->r, pnt.x, y_min, x_max, y_max);
				
				drawLine(node, pnt.x, y_min, pnt.x, y_max);
			} else {
				drawPlanes(node->l, x_min, y_min, x_max, pnt.y);
				drawPlanes(node->r, x_min, pnt.y, x_max, y_max);
				
				drawLine(node, x_min, pnt.y, x_max, pnt.y);
			}
		}
	}
	

	void drawPoints(KDNode *node) {
		if(node->isLeaf()) {
			ofCircle(node->pnt, 3);
			//ofDrawBitmapString(ofToString(node->pnt.x)+","+ofToString(node->pnt.y), node->pnt + ofVec2f(0,20));
		} else {
			drawPoints(node->l);
			drawPoints(node->r);
		}
	}

	void drawLine(KDNode *node, float x_min, float y_min, float x_max, float y_max) {
		float lwidth = max_depth - node->depth+1;
		ofSetLineWidth(lwidth);
		ofLine(x_min, y_min, x_max, y_max);
	}
	
	void drawGrid() {
		ofSetColor(190,190,190,255);
		drawPlanes(root, 0, 0, ofGetWidth(), ofGetHeight());
		ofSetLineWidth(1.0);
		ofFill();
		ofSetColor(255,0,0);
		drawPoints(root);
	}
};
		
class NN {
public:
	ofVec2f pnt_in;
	ofVec2f pnt_nn;
	float min_sq;
	
	NN(ofVec2f _pnt_in) {
		pnt_in = _pnt_in;
		min_sq = numeric_limits<float>::max();
	}
	
	void update(KDNode *node) {
		
		ofVec2f dv = node->pnt - pnt_in;
		float cur_sq = dv.lengthSquared();
		
		if(cur_sq < min_sq) {
			min_sq = cur_sq;
			pnt_nn = node->pnt;
		}
	}
	
	void getNN(NN* near, KDNode *node) {
		if(node->isLeaf()) {
			near->update(node);
		} else {
			
			float dist_hp = planeDistance(node, near->pnt_in);
			
			// check the half-space, the point is in.
			getNN(near, (dist_hp < 0) ? node->l : node->r);
			
			// check the other half-space when the current distance (to the
			// nearest-neighbor found so far) is greater, than the distance
			// to the other (yet unchecked) half-space's plane.
			if((dist_hp * dist_hp) < near->min_sq ) {
				getNN(near, (dist_hp < 0) ? node->r : node->l);
			}
			
		}
	}
	
	float planeDistance(KDNode *node, ofVec2f point) {
		if((node->depth & 1)) {
			return point.y - node->pnt.y;
		} else {
			return point.x - node->pnt.x;
		}
	}
	
};

};
		
#endif
