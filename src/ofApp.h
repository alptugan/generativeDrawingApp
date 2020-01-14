#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxFilikaUtils.h"

class ofApp : public ofBaseApp{

	public:
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
		
    // GUI
    bool isDebug;
    ofxPanel gui;
    ofParameter<int> gDrawingMode;
    ofParameter<bool> gEnableRotation;
    ofParameter<bool> gEnableAutoClear;
    ofParameter<bool> gDisableInteraction;
    ofParameter<bool> gShowLight;
    ofParameter<bool> gEnableLight;
    ofParameter<bool> gEnableDepthTest;
    ofParameter<bool> gEnableCameraTracking;
    ofParameter<glm::vec3> gCameraTrackingOff;
    ofParameter<ofColor> gPathColor;
    ofParameter<ofColor> gConnectionColor;
    ofParameter<float> gAnimationSpeed, gRotationSpeed;
    
    ofParameter<float> gDistanceThres;
    ofParameter<int> gLineW;
    ofParameter<glm::vec3> gNoiseVals;
    ofParameter<glm::vec3> gLightPos;
    ofParameter<glm::vec3> gLightAreaPos;
    
    float pMx, pMy, pMz, count;
    float xC;
    float yC;
    float zC;
    vector<glm::vec3> points;

    ofxFilikaUtils utils;
    
    
    ofMesh meshPrimary;
    ofMesh meshConnection;
    ofMesh meshGlobe;
    void stroke(float x, float y, float z);
    void clearCanvas();
    float rad;
    
    // 3D SCENE CAM
    ofEasyCam cam;
    ofLight light;
    ofLight lightArea;
};
