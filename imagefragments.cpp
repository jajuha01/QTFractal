/*
** Copyright (c) 2014 Jarmo Hautakoski
**
** Fractal project
**  */

#include "imagefragments.h"

ImageFragments::ImageFragments(PixelCalculationThread* fragment)
{
    table = fragment->GetCopyofData(&sizeX,&sizeY,&startX,&startY,&segmentId);
}

ImageFragments::~ImageFragments()
{
    free(table);
}

