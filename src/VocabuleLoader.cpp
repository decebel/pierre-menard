//
//  VocabuleLoader.cpp
//  ImageComparisonShader_image
//
//  Created by Matthew Fargo on 2014/08/27.
//
//

#include "VocabuleLoader.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

VocabuleLoader::VocabuleLoader() {
}
VocabuleLoader::~VocabuleLoader() {}

void VocabuleLoader::loadImages(string lex) {
    ofBuffer buffer = ofBufferFromFile(lex+".txt");
    string s = buffer.getText();
    if (s.size()>0) {
        ofStringReplace(s, ".", "");
        ofStringReplace(s, ",", "");
        ofStringReplace(s, "!", "");
        ofStringReplace(s, "?", "");
        ofStringReplace(s, "\"", "");
        ofStringReplace(s, "'", "");
        ofStringReplace(s, "&", "");
        ofStringReplace(s, "*", "");
        ofStringReplace(s, "$", "");
        ofStringReplace(s, "@", "");
        ofStringReplace(s, ":", "");
        ofStringReplace(s, ";", "");
        ofStringReplace(s, "%", "");
        ofStringReplace(s, "-", "");
        ofStringReplace(s, "(", "");
        ofStringReplace(s, ")", "");
        ofStringReplace(s, "[", "");
        ofStringReplace(s, "]", "");
        _lex = lex;
        _searchTerms.clear();
        vector<string> v = ofSplitString(s, " ");
        for (int i=0; i<v.size(); i++)
            if (std::find(_searchTerms.begin(), _searchTerms.end(), v[i]) == _searchTerms.end())
                _searchTerms.push_back(v[i]);
        startThread();
    }
    else printf("ImageComparisonShader::Warning -> no file named %s,txt",lex.c_str());

}

//https://www.googleapis.com/customsearch/v1?key=AIzaSyDd8jnYdKRyVxSmQs-1hxe5qS7c0aSnfGo&cx=014871620488843956810:dktjllzbbso&q=dog&searchType=image&imgType=lineart&imgColorType=mono&imgSize=small&safe=off&googlehost=google.co.jp

void VocabuleLoader::_saveHttpsImageWithPoco(string httpsUrl, int index, string term) {
    //try {
    Poco::URI uri(httpsUrl);
    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";
    
    Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE) );
    
    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
    Poco::Net::HTTPRequest req( Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
    req.setKeepAlive(false);
    
    session.setTimeout( Poco::Timespan(30, 0) );
    
    session.sendRequest(req);
    Poco::Net::HTTPResponse res;
    istream& rs = session.receiveResponse(res);
    
    //vector<string> v = ofSplitString(httpsUrl, ".");
    //string suffer = v[v.size()-1];
    string suffer =  "jpg"; /// google always saves thumbs as jpeg?
    string outUri = "menard/" + term + "_"+ofToString(index)+"."+suffer;
    printf(" OUTPUT: %s\n", outUri.c_str());
    ofstream outfile;
    outfile.open( ofToDataPath(outUri).c_str(), ios_base::binary);
    
    Poco::StreamCopier::copyStream( rs, outfile );
    //Poco::StreamCopier::copyStream(*pStr.get(), myfile);
    
    outfile.close();
    ///}
    //catch (int e) {
    //    printf(" ERROR IS HUMAN\n");
    //}
    
}

    // can only do 10 at a time so
bool VocabuleLoader::_loadImages(string term, int startIndex) {
    
    string url;
    if (startIndex>1) url = "https://www.googleapis.com/customsearch/v1?key=AIzaSyDd8jnYdKRyVxSmQs-1hxe5qS7c0aSnfGo&cx=014871620488843956810:dktjllzbbso&q="+term+"&searchType=image&start=" + ofToString(startIndex)+"&imgType=lineart&imgColorType=mono&googlehost=google.co.jp";
    else url = "https://www.googleapis.com/customsearch/v1?key=AIzaSyDd8jnYdKRyVxSmQs-1hxe5qS7c0aSnfGo&cx=014871620488843956810:dktjllzbbso&q="+term+"&searchType=image&imgType=lineart&imgColorType=mono&googlehost=google.co.jp";
    cout << "URL: " << url;
    
    _json.clear();
    if (_json.open(url)) {
        int numImages = _json["items"].size();
        
        for(int i=0; i< numImages; i++) {
            string url = _json["items"][i]["image"]["thumbnailLink"].asString();
            _saveHttpsImageWithPoco(url, startIndex+i, term);
            /*
             ofImage image;
             if (image.loadImage(_json["items"][i]["image"]["thumbnailLink"].asString())) {
             _xml.addValue("url", _json["items"][i]["image"]["thumbnailLink"].asString());
             image.saveImage("menard/"+_searchTerms[_termIndex]+"_"+ofToString(i+_startIndex)+".png");
             //_images.push_back(image);
             }*/
        }
        if (_json["queries"]["nextPage"].size()>0) {
            printf(" No more\n");
            return false;
        }
        // next
        return true;
    }
    else {
        printf(" couldn't make request\n");
        return false;
    }
}

void VocabuleLoader::threadedFunction() {
    int wordIndex = 0;
    int startIndex = 1;
    while(isThreadRunning()) {
        bool didLoad = _loadImages(_lex, startIndex);
        //printf(" Loaded %s (%d)\n", _searchTerms[wordIndex].c_str(), startIndex);
        if (didLoad && startIndex<90) {
            startIndex += 10;
        }
        else {
            startIndex = 1;
            wordIndex++;
            if (wordIndex>= _searchTerms.size()) break;
        }
        printf(" YO: %d %d", wordIndex, startIndex);
    }
    ofEventArgs e;
    ofNotifyEvent(this->ON_IMAGES_COMPLETE, e);
}
