//
//  rangeutil.h
//  Wii2Osc
//
//  Created by Manuel Hirschauer on 18.04.15.
//
//

#ifndef __Wii2Osc__RangeUtil__
#define __Wii2Osc__RangeUtil__

#include <stdio.h>

class RangeUtil
{
public:
    RangeUtil( float sourcemin = 0.0, float sourcemax = 1.0, float targetmin = 0.0, float targetmax= 1.0 );
    
    float getValue(float x);
    void test();
    
private:                             // privat
    float targetmin;
    float targetmax;
    float sourcemin;
    float sourcemax;
    float targetoffset = 0.0;
    float sourceoffset = 0.0;
};

#endif /* defined(__Wii2Osc__rangeutil__) */
