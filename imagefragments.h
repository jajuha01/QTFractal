/*
** Copyright (c) 2014 Jarmo Hautakoski
**
** Fractal project
**  */

#ifndef IMAGEFRAGMENTS_H
#define IMAGEFRAGMENTS_H

#include <pixelcalculationthread.h>

class ImageFragments
{
public:
    ImageFragments();
    ImageFragments(PixelCalculationThread* fragment);
    ~ImageFragments();
    ImageFragments& operator =(const ImageFragments & a);
    void CopyDataToBuffer(QImage);
    int segmentId;
    unsigned int* table;
    int sizeX;
    int sizeY;
    int startX;
    int startY;
};

#endif // IMAGEFRAGMENTS_H
