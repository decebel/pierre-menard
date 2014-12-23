//
//  ThumbFilter.h
//  ImageComparisonShader
//
//  Created by Matthew Fargo on 2014/08/14.
//
//

#ifndef __ImageComparisonShader__ThumbFilter__
#define __ImageComparisonShader__ThumbFilter__

#include "AbstractTwoInputFilter.h"

class ThumbFilter : public AbstractTwoInputFilter {
public:
	ThumbFilter(float width, float height);
	virtual ~ThumbFilter();

private:
    virtual string  _getVertSrc();
    virtual string  _getFragSrc();

};

#endif /* defined(__ImageComparisonShader__ThumbFilter__) */
