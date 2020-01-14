#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLogToConsole();
    
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofSetSmoothLighting(true);
    
    // do not forget to replace the following path your own ttf/otf font file path
    ofxGuiSetFont( "../../../../../../assets/DIN.otf", 8 );
    ofxGuiSetDefaultWidth( 300 );
    ofxGuiSetFillColor(ofColor(255,204,0,200));
    
    string xmlSettingsPath = "generativeDrawing_settings.xml";
    gui.setup( "Drawing Parameters", xmlSettingsPath );
    gui.add(gDrawingMode.set("Drawing Mode", 0, 0, 4)); // Number of Tentacles
    gui.add(gEnableCameraTracking.set("Enable Target Tracking", false));
    gui.add(gCameraTrackingOff.set("Camera Target Offset", glm::vec3(0,0,260), glm::vec3(-1000), glm::vec3(1000)));
    gui.add(gEnableAutoClear.set("Enable Auto Clear Canvas", true));
    gui.add(gEnableRotation.set("Enable Canvas Rotation", false));
    gui.add(gDisableInteraction.set("Disable Interaction", true));
    gui.add(gDistanceThres.set("Distance Threshold", 150, 5, 1000));
    gui.add(gLineW.set("Line Width", 2, 1, 4));
    gui.add(gNoiseVals.set("Noise Multipliers", glm::vec3(0.002), glm::vec3(0.00001, 0.00001, 0.00001), glm::vec3(20)));
    gui.add(gEnableDepthTest.set("Enable Depth Test", true));
    gui.add(gEnableLight.set("Enable Lighting", true));
    gui.add(gShowLight.set("Show Light", true));
    gui.add(gLightPos.set("Object Light Position", glm::vec3(0), glm::vec3(-3000), glm::vec3(3000)));
    gui.add(gLightAreaPos.set("Area Light Position", glm::vec3(0), glm::vec3(-5000), glm::vec3(5000)));
    gui.add(gAnimationSpeed.set("Animation Speed", 0.2, 0.01, 3));
    gui.add(gRotationSpeed.set("Rotation Speed", 6, 1, 30));
    gui.add(gPathColor.set("Path Color", ofColor(255), ofColor(0), ofColor(255)));
    gui.add(gConnectionColor.set("Connection Color", ofColor(255), ofColor(0), ofColor(255)));
    gui.setPosition(0, 0);
    gui.loadFromFile(xmlSettingsPath);
    
    // Set background color
    ofBackground(0);
    
    count = 0;
    //points.resize(1);
    
    
    
    
    
    
    lightArea.setAreaLight(ofGetWidth(), ofGetWidth());
    lightArea.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    lightArea.setDiffuseColor(ofFloatColor(0.8, 0.8, 0.8));
    lightArea.setAmbientColor(ofFloatColor(0.2, 0.2, 0.2));
    lightArea.enable();
    
    rad = 4;
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetLineWidth(gLineW);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    
    
    //ofSetColor(0);
    //ofLog() << meshPrimary.getCentroid();
    float t = ofGetElapsedTimef() * gAnimationSpeed;
    vector<glm::vec3> & mps = meshPrimary.getVertices();
    // Enbale autoclear for performance optimization
    // After some amount of vertices number, we need to clear vertex buffer
    if(gEnableAutoClear && ofGetFrameRate() < 40 && points.size() > 100) {
        ofLog() << "mesh vertex num: " << points.size();
        clearCanvas();
    }
    
    
    if(gEnableDepthTest)
        ofEnableDepthTest();
    
    ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    // Draw light
    if(gEnableLight)
        light.setPosition(gLightPos->x, gLightPos->y, gLightPos->z);
    
    if(gShowLight && gEnableLight)
        light.draw();
    
    // Camera tracking
    if(mps.size() > 0 && gEnableCameraTracking)
    {
        auto target = glm::vec3(mps[mps.size() - 1].x, mps[mps.size() - 1].y, mps[mps.size() - 1].z);
        auto camera_location = target + gCameraTrackingOff.get();
    
        this->cam.setPosition(camera_location);
        this->cam.setTarget(target);
        this->cam.lookAt(target);
        
        //light.setPosition(camera_location);
    }
    
    cam.begin();
    
    // Enable Light
    if(gEnableLight) {
        light.enable();
        ofEnableLighting();
    }
    
    ofPushMatrix();
    
    // Sphere light
    lightArea.setPosition(gLightAreaPos);
    
    
    // Draw huge sphere
    ofPushMatrix();
        glPointSize(1);
        ofSetColor(255);
        ofRotateRad(ofDegToRad(ofGetElapsedTimef()*gRotationSpeed/2), 0, 1, 0);
        meshGlobe = ofMesh::sphere(ofGetWidth()*1.5, 48, ofPrimitiveMode::OF_PRIMITIVE_TRIANGLE_STRIP);
        //meshGlobe.drawWireframe();
        meshGlobe.drawVertices();
        ofSetColor(0);
        meshGlobe.drawFaces();
    ofPopMatrix();
    
    if(gEnableRotation)
    {
        ofRotateRad(ofDegToRad(-ofGetElapsedTimef()*gRotationSpeed), 1, 1, 1);
    }
    
    ofSetColor(255,30);
    
    if(gDrawingMode == 0) {
        float xp = cos(t) * rad;
        float yp = sin(t) * rad;
       
        
        stroke(xp, yp, 0);
        
        rad = rad + 0.7;
    }
    
    else if(gDrawingMode == 1) {
        
        xC = ofSignedNoise(gNoiseVals->x, 0, t) * 650;
        yC = ofSignedNoise(0, gNoiseVals->y, t) * 500;
        zC = ofSignedNoise(t, 0, gNoiseVals->z) * 1000;
        
        stroke(xC, yC, zC);
    }
    
    meshConnection.draw();
    meshPrimary.draw();
    

    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofDrawSphere(mps[mps.size() - 1].x, mps[mps.size() - 1].y, mps[mps.size() - 1].z, 5);
    ofPopStyle();
    
    
    ofPopMatrix();
    
    if(gEnableLight) {
        ofDisableLighting();
        light.disable();
    }
    cam.end();
    
    
    ofDisableDepthTest();
    
    // DEBUG
    if(isDebug)
    {
        gui.draw();
        utils.drawFPS(ofxFilikaAlignment::TOP_RIGHT);
    }
    
}

void ofApp::stroke(float x, float y, float z) {
    float d;
    //ofLog() << points[0][0] << " " << points[count][0];
    float op;
    //meshConnection.clear();
    
    // Remove vector from begining to optimize
    /*if(meshPrimary.getVertices().size() > 100) {
        ofLog() << points.size();
        points.erase(points.begin(),points.begin() + 1);
        
        meshPrimary.removeVertex(0);
        meshPrimary.removeColor(0);
        
        meshConnection.removeVertex(0);
        meshConnection.removeColor(0);
        meshConnection.removeVertex(1);
        meshConnection.removeColor(1);
    }*/
    
    // thickness of 40
    /*ofPoint a = ofPoint(100,100);
    ofPoint b = ofPoint(400,400);
    ofPoint diff = (b - a).getNormalized();
    diff.rotate(90, ofPoint(0,0,1));
    ofMesh m;
    m.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    m.addVertex(a + diff * 20.0);
    m.addVertex(a - diff * 20.0);
    m.addVertex(b + diff * 20.0);
    m.addVertex(b - diff * 20.0);
    m.draw();*/
    
    for (int i = 0; i < points.size(); i++) {
        
        glm::vec3 m;
        m.x = x;
        m.y = y;
        m.z = z;
        
        d = glm::length(m - points[i]);
        
        float rand = ofRandom(0,1);
        
        if(d < gDistanceThres && rand > 0.2 && rand < 0.34) {
            op = ofMap(d, 0, gDistanceThres, 255, 0);

            meshConnection.setMode(OF_PRIMITIVE_LINES);
            meshConnection.addColor(ofColor(gConnectionColor->r,gConnectionColor->g,gConnectionColor->b,op));
            meshConnection.addVertex(m);
            
            meshConnection.addColor(ofColor(gConnectionColor->r,gConnectionColor->g,gConnectionColor->b,op));
            meshConnection.addVertex(points[i]);
            
        }
    }
    
    
    meshPrimary.setMode(OF_PRIMITIVE_LINES);
    float mapDepthCl = ofMap(z, -1000, 1000, 4, 1);
    
    if(meshPrimary.getVertices().size() == 0) {
        pMx = x;
        pMy = y;
        pMz = z;
    }
    
    meshPrimary.addVertex(glm::vec3(pMx, pMy, pMz));
    meshPrimary.addColor(ofColor(gPathColor->r,gPathColor->g,gPathColor->b,gPathColor->a));
    
    meshPrimary.addVertex(glm::vec3(x, y, z));
    meshPrimary.addColor(ofColor(gPathColor->r,gPathColor->g,gPathColor->b,gPathColor->a));
    
    points.push_back(glm::vec3(x, y, z));
    
    
    pMx = x;
    pMy = y;
    pMz = z;
    
    
}

void ofApp::clearCanvas() {
    meshConnection.clear();
    meshPrimary.clear();
    points.clear();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f') {
        ofToggleFullscreen();
    }else if(key == 'c') {
        clearCanvas();
    }else if(key == 'd') {
        isDebug ^= true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(!gDisableInteraction)
        stroke(x, y, 0);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //points.push_back(glm::vec3(x, y, 0));
    
    if(!gDisableInteraction)
    {
        pMx = x;
        pMy = y;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    count++;
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
