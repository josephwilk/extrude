#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSVG.h"
#include "ofxClipper.h"
#include "ofxConvexHull.h"
#include "ofxSweepLine.h"
#include "ofxMSAPerlin.h"


class ofApp : public ofBaseApp{
  vector<glm::vec3> personVerts;
  vector<glm::vec3> chairVerts;



  bool triggerUpdate;

  vector<vector<ofPolyline>> allLines;


  /*-----ofxSweepLine-----*/
  ofxSweepLine sweepLine;
  vector<glm::vec2> intersections;



  vector<ofPath> svgPolyLayers;


  ofxConvexHull convexHull;

  vector<ofPoint> points;
  vector<ofPoint> hull;


    vector<string> sentences;
//    ofTrueTypeFont text;
 ofx::Clipper clipper;
  vector<ofTrueTypeFont> fonts;

  int frame=0;
  int i;
  int i2;
  vector<ofPolyline> polys;
  ofxIntSlider sandGrain;
  shared_ptr<ofBaseRenderer> def;
  shared_ptr<ofCairoRenderer> cairo;
  bool toCVG;
  ofxSVG svg;

  ofxSVG svgLayer;

  ofxPanel gui;
  ofPolyline person;
  ofPolyline chair;
  ofPolyline personSkeleton;
  ofPolyline personSkeleton2;
  ofPolyline chairSkeleton;

  ofxIntSlider chairSpacing;
  ofxIntSlider wheelSpacing;

  ofxFloatSlider lineSpacing;


  ofxFloatSlider letterSpacing;

  ofxFloatSlider fontFactor;
  ofxFloatSlider smoothShape;

  ofxFloatSlider noiseRatio;
  ofxFloatSlider noiseMin;
  ofxFloatSlider noiseMax;

  ofxFloatSlider fudge;

  ofxIntSlider startSmoothing;

  ofxIntSlider chanceGitch;
  ofxIntSlider smoothed;
  ofxIntSlider strokes;

  ofxIntSlider fontStart;
  ofxIntSlider fontInc;


  ofxFloatSlider rejectRate;
  ofxFloatSlider forceRadius;


  ofxIntSlider octaves;
  ofxFloatSlider freq;
  ofxFloatSlider amp;
  ofxIntSlider seed;
  ofxIntSlider noiseStart;


  ofxToggle outline;
  ofxToggle letters;
  ofxToggle noiseOut;
  ofxToggle fontScale;


	public:

    ofPolyline removeIntersects(ofPolyline p);
  void drawLetters(ofPolyline line, int strokeCount);
  ofPolyline burbPolyline(ofPolyline l, int s, int dir);


		void setup();
		void update();
		void draw();

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
		
};
