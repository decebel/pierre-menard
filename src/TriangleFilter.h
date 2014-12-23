//
//  TriangleFilter.h
//  ImageComparisonShader
//
//  Created by Matthew Fargo on 2014/08/14.
//
//

#ifndef __ImageComparisonShader__TriangleFilter__
#define __ImageComparisonShader__TriangleFilter__

#include "AbstractTwoInputFilter.h"

class TriangleFilter : public AbstractTwoInputFilter {
public:
	TriangleFilter();
	virtual ~TriangleFilter();

private:
    virtual string  _getFragSrc();

};

#endif /* defined(__ImageComparisonShader__TriangleFilter__) */
