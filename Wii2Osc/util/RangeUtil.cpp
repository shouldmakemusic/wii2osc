//
//  rangeutil.cpp
//  Wii2Osc
//
//  Created by Manuel Hirschauer on 18.04.15.
//
//

#include "RangeUtil.h"
#include <cmath>

RangeUtil::RangeUtil( float sourcemin, float sourcemax, float targetmin, float targetmax )
{
    if (targetmin != 0.0) {
        targetoffset = targetmin;
        targetmax -= targetoffset;
        targetmin -= targetoffset;
    }
    if (sourcemin != 0.0)
    {
        sourceoffset = sourcemin;
        sourcemax -= sourceoffset;
        sourcemin -= sourceoffset;
    }
    this->targetmin = targetmin;
    this->targetmax = targetmax;
    this->sourcemin = sourcemin;
    this->sourcemax = sourcemax;
}

float RangeUtil::getValue(float x)
{
    x = x - sourceoffset;
    float value = (x * targetmax / sourcemax) + targetoffset;
    return value ;
}

void RangeUtil::test()
{
    RangeUtil ru;
    printf("sourcemin: %f, sourcemax: %f, targetmin: %f, targetmax: %f\n", ru.sourcemin, ru.sourcemax, ru.targetmin, ru.targetmax);
    
    for(double i = 0; i <= 10; ++i)
    {
        printf("%f gets %f\n", i/10, ru.getValue(i/10));
    }
    
    RangeUtil ru2(0, 10, 4, 6);
    printf("sourcemin: %f, sourcemax: %f, targetmin: %f, targetmax: %f\n", 0.0, 10.0, 4.0, 6.0);
    
    for(double i = 0; i <= 10; ++i)
    {
        printf("%f gets %f\n", i, ru2.getValue(i));
    }
    
    RangeUtil ru3(-10, 10, 4, 6);
    printf("sourcemin: %f, sourcemax: %f, targetmin: %f, targetmax: %f\n", -10.0, 10.0, 4.0, 6.0);
    
    for(double i = -10; i <= 10; ++i)
    {
        printf("%f gets %f\n", i, ru3.getValue(i));
    }
    
}

