//
//  VocabuleManager.cpp
//  ImageComparisonShader_image
//
//  Created by Matthew Fargo on 2014/08/25.
//
//

#include "VocabuleManager.h"


VocabuleManager::VocabuleManager() {
    
}
VocabuleManager::~VocabuleManager() {}


void VocabuleManager::loadWord(string word) {
    _directory = ofDirectory(word);
    if (_directory.exists()) {
        
        _vocabule.word = word;
        _vocabule.maru = 0;
        _vocabule.batu = 0;
        _startIndex = 0;
        _termIndex = 0;

        //_directory.allowExt("jpg");
        _directory.listDir();
        printf(" NUM PICS %d\n", _directory.numFiles() );
        _currentImageIndex = floorf(ofRandom(_directory.numFiles()));
        /*
         while (_searchTerms[_termIndex] != "ficciones") {
         //printf(" DON WIT %s\n", _searchTerms[_termIndex].c_str());
         _termIndex++;
         }
         */
    }
    else printf("VocabuleManager::Warning -> no directory named %s", word.c_str());
}

void VocabuleManager::createImageCollectionForWord(string word) {
    if (!_loader.isThreadRunning()) {
        _loader.loadImages(word);
    }
    else printf("VocabuleManager::Warning -> already loading images");
}



void VocabuleManager::nextImage() {
    _currentImageIndex++;
    if (_currentImageIndex>_directory.numFiles()-1) _currentImageIndex = 0;
    _currentImage.loadImage(_directory.getFile(_currentImageIndex));
}


