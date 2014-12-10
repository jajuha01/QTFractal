/*
** Copyright (c) 2014 Jarmo Hautakoski
**
** Fractal project
**  */

#include "imagefragments.h"

ImageFragments::ImageFragments()
{
    segmentId = 0;
    table = NULL;
    sizeX = 0;
    sizeY = 0;
    startX = 0;
    startY = 0;
}

ImageFragments::ImageFragments(PixelCalculationThread* fragment)
{
    table = fragment->GetCopyofData(&sizeX,&sizeY,&startX,&startY,&segmentId);
}

ImageFragments::~ImageFragments()
{
    delete[] table;
}

ImageFragments& ImageFragments::ImageFragments::operator =(const ImageFragments &a)
{
    int size = a.sizeY*a.sizeX;
    memcpy(this,&a,sizeof(a));
    table = new unsigned int[size];
    memcpy(table,a.table,size*sizeof(unsigned int));
    return *this;
}

