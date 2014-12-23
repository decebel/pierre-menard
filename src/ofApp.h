//
//  ofApp.h
//  ofxFilterLibraryExample
//
//  Created by Matthew Fargo on 2014/06/25.
//
//
//  This is an example of how to use all the filters in this library.
//  They should be as easy as coffee filters, but not as easy as
//  cigarette filters.

#pragma once

#include "ofMain.h"
#include "ofxFilterLibrary.h"
#include "DifferenceFilter.h"
#include "ThumbFilter.h"
#include "FaceFilter.h"
#include "VocabuleManager.h"
#include "ofxLogger.h"


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void                        keyPressed(int key);


private:

    bool                        _isLineDrawing;
    
    void                        _nextImage();
    void                        _iterate();
    
    int                         numImprovements;
    unsigned long               totalAttempts;
    
        // image search stuff
    VocabuleManager             _vocabuleManager;
    
    
    DifferenceFilter *          _differenceFilter;
    
    AbstractTwoInputFilter *    _thumbFilter;
    
    ofImage                     _ideal;
    ofFbo                       _incumbant, _candidate, _difference, _incumbantSample, _shrinkage;
    ofVec2f                     _size;
    int                         _newPolygons;
    
};

