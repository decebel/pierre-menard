#include "ofApp.h"

const float SHRINKAGE_FACTOR = 1.0f; // same framerate for 0.1 and 0.01!!

void ofApp::setup(){
    
    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetFrameRate(120);
 
    _isLineDrawing = false;
    _ideal.loadImage("img/matthewfargo.png");

    ofVec2f size = ofVec2f(_ideal.getWidth(), _ideal.getHeight());
    if (size.y> 1000.f) size *= 1000.f/size.y;
    ofSetWindowShape(size.x, size.y);

    
    ofVec2f MAX_IMAGE_SIZE = ofVec2f(256, 256); //arbitrary

    
    _size = ofVec2f(_ideal.width, _ideal.height);
    _incumbant.allocate(_ideal.width, _ideal.height);
    _incumbant.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(255);
    ofSetColor(0);
    ofRect(0, 0, _ideal.width, _ideal.height);
    _incumbant.end();
    _candidate.allocate(_ideal.width, _ideal.height);
    _difference.allocate(MAX_IMAGE_SIZE.x, MAX_IMAGE_SIZE.y);
    _shrinkage.allocate(_difference.getWidth()*SHRINKAGE_FACTOR, _difference.getHeight()*SHRINKAGE_FACTOR);
    _incumbantSample.allocate(_difference.getWidth(), _difference.getHeight());
    _differenceFilter = new DifferenceFilter(_incumbant.getTextureReference(), _ideal.getTextureReference());
    _isLineDrawing = true;
    if (_isLineDrawing)
        _thumbFilter = new ThumbFilter(_ideal.width, _ideal.height);
    else
        _thumbFilter = new FaceFilter(_ideal.width, _ideal.height);
    
    numImprovements = 0;
    totalAttempts = 0;
    _newPolygons = 0;
    string term = "brush";
    _vocabuleManager.loadWord(term);
    
    //ofxLog::setFilePath(term + ".txt");
    ofxLog::enableFile();
    ofxLog() << "New Session" << endl << endl;
    
}

void ofApp::_iterate() {
    
    //　比較シェーダと三角シェーダを切り分ける
    //　三角をブレンディングで描いて、３画の結果を比較して、採用の場合は三角の結果をそのままincumbantに移す。
    _vocabuleManager.nextImage();
    ofImage thumb = _vocabuleManager.getCurrentImage();
    
    int rotations = floorf(ofRandom(4.f));
    if (rotations>0) thumb.rotate90(rotations);
    
    if (ofRandomuf()>0.5) thumb.mirror(true, false);
    if (ofRandomuf()>0.5) thumb.mirror(false, true);
    
    float MIN_PERCENT = 0.5;
    float MAX_PERCENT = 1.0;
    
    float thumbScale = ofRandom(MIN_PERCENT, MAX_PERCENT);
    ofVec2f offsetAmount = ofVec2f(thumb.width/(float)_ideal.width, thumb.height/(float)_ideal.height) * thumbScale;
    _thumbFilter->setSecondTexture(thumb.getTextureReference());
    _thumbFilter->updateParameter("thumbscale", thumbScale);
    if (_isLineDrawing)
        _thumbFilter->updateParameter("color", ofVec4f(ofRandomuf(), ofRandomuf(), ofRandomuf(), 0.5));
    float tx = -offsetAmount.x + ofRandom(1.0 + offsetAmount.x);
    float ty = -offsetAmount.y + ofRandom(1.0 + offsetAmount.y);
    _thumbFilter->updateParameter("thumbrect", ofVec4f(tx, ty, thumb.width, thumb.height));
    
    _candidate.begin();
    //ofClear(0, 0, 0, 0);
    _thumbFilter->begin();
    _incumbant.draw(0, 0);
    _thumbFilter->end();
    _candidate.end();

    _difference.begin();
    _differenceFilter->begin();
    _candidate.draw(0, 0);
    _differenceFilter->end();
    _difference.end();
    
    _shrinkage.begin();
    _difference.draw(0, 0, _shrinkage.getWidth(), _shrinkage.getHeight());
    _shrinkage.end();
    
    ofFloatPixels pix;
    _shrinkage.readToPixels(pix);

    totalAttempts++;
    int numPixels = pix.getWidth()*pix.getHeight();
    
    float cumulative = 0.f;
    
    for (int y=1; y<pix.getHeight(); y+=3) {
        for (int x=1; x<pix.getWidth(); x+=3) {
            float r = pix.getColor(x, y).r;
            if (r> 0.75) cumulative += pix.getColor(x, y).g;
            else if (r < 0.25) cumulative -= pix.getColor(x, y).g;
        }
    }
    if (cumulative < 0.f) {
        _newPolygons++;
        numImprovements++;
        _incumbant.begin();
        _candidate.draw(0, 0);
        _incumbant.end();
    }
}

void ofApp::update() {
    

    //if (_images.size()==0) _getImages("dog");
    for ( int i=0; i<1; i++) {
        _iterate();
    }
}

void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofSetColor(255);



    //ofPushMatrix();
    //ofScale(0.5, 0.5);

    /*
    _incumbant.draw(_ideal.width, 0);
    _ideal.draw(0, 0, _ideal.width, _ideal.height);
    _difference.draw(0, _ideal.height);
    _candidate.draw(_ideal.width, _ideal.height);
    */
    
    
    _incumbant.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    //ofTranslate(0, _ideal.height);
    
    //ofPopMatrix();

    int numForScreenCap = (numImprovements<1000) ? 80 : (numImprovements<10000) ? 200 : 400;
    if (_newPolygons>numForScreenCap) {
        
        
        ofPixels p;
        _incumbant.readToPixels(p);
        ofImage screenImage;
        screenImage.setFromPixels(p);
        //screenImg.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
        //screenImg.grabScreen(0,0,ofGetWidth(), ofGetHeight());
        screenImage.saveImage("output/image_"+ofToString(ofGetFrameNum())+".png");
        _newPolygons -= numForScreenCap;
        ofxLog() << "frame " << ofToString(ofGetFrameNum()) << ", " << numImprovements << "/" << totalAttempts;
    }

    //ofDrawBitmapString( _searchTerms[_termIndex].c_str(), ofPoint(40, 40));
    ofDrawBitmapString( ofToString(ofGetFrameRate()) + ": numImprovements = " + ofToString(numImprovements), ofPoint(40, 20));
    
}

void ofApp::keyPressed(int key){

    if (key=='f') ofToggleFullscreen();
    //if (key==' ') _getImages();

}
