//
//  VocabuleLoader.h
//  ImageComparisonShader_image
//
//  Created by Matthew Fargo on 2014/08/27.
//
//

#ifndef __ImageComparisonShader_image__VocabuleLoader__
#define __ImageComparisonShader_image__VocabuleLoader__

#include "ofMain.h"
#include "ofxJSONElement.h"


class VocabuleLoader : public ofThread {
public:
	VocabuleLoader();
	virtual ~VocabuleLoader();

    ofEvent<ofEventArgs>    ON_IMAGES_COMPLETE;
    
    void    loadImages(string lex);
    void    threadedFunction();
    
private:
    
    ofxJSONElement              _json;
    string                      _lex;
    vector<string>              _searchTerms;
    bool    _loadImages(string lex, int startIndex);
    void    _saveHttpsImageWithPoco(string httpsUrl, int index, string term);
    

};

#endif /* defined(__ImageComparisonShader_image__VocabuleLoader__) */
