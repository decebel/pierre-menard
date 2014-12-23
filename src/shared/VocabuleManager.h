//
//  VocabuleManager.h
//  ImageComparisonShader_image
//
//  Created by Matthew Fargo on 2014/08/25.
//
//

#ifndef __ImageComparisonShader_image__VocabuleManager__
#define __ImageComparisonShader_image__VocabuleManager__

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "VocabuleLoader.h"

typedef struct {
    string  word;
    unsigned long       maru;
    unsigned long       batu;
} Vocabule;


class VocabuleManager {
public:
	VocabuleManager();
	virtual ~VocabuleManager();
    
    void    createImageCollectionForWord(string word);
    
    void    loadWord(string word);
    void    nextImage();
    ofImage &                   getCurrentImage() { return _currentImage; }
    
    

protected:
    
    bool                        _getImages();
    Vocabule                    _vocabule;
    
    ofImage                     _currentImage;
    vector<string>              _urls;
    int                         _startIndex, _termIndex, _currentImageIndex;
    ofxXmlSettings              _xml;
    ofDirectory                 _directory;

    VocabuleLoader              _loader;
};

#endif /* defined(__ImageComparisonShader_image__VocabuleManager__) */
