//
//  DifferenceFilter.h
//  ImageComparisonShader
//
//  Created by Matthew Fargo on 2014/08/09.
//
//

#ifndef __ImageComparisonShader__DifferenceFilter__
#define __ImageComparisonShader__DifferenceFilter__

#include "AbstractTwoInputFilter.h"

class DifferenceFilter : public AbstractTwoInputFilter {
public:
    DifferenceFilter(ofTexture & incumbant, ofTexture & ideal);
	virtual ~DifferenceFilter();

protected:
    
    ofTexture       _ideal;
    virtual string  _getVertSrc();
    virtual string  _getFragSrc();

};

#endif /* defined(__ImageComparisonShader__DifferenceFilter__) */
