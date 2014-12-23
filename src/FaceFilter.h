//
//  FaceFilter.h
//  ImageComparisonShader
//
//  Created by Matthew Fargo on 2014/08/14.
//
//

#ifndef __ImageComparisonShader__FaceFilter__
#define __ImageComparisonShader__FaceFilter__

#include "AbstractTwoInputFilter.h"

class FaceFilter : public AbstractTwoInputFilter {
public:
	FaceFilter(float width, float height);
	virtual ~FaceFilter();

private:
    virtual string  _getVertSrc();
    virtual string  _getFragSrc();

};

#endif /* defined(__ImageComparisonShader__FaceFilter__) */
