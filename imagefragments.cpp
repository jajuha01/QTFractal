#include "imagefragments.h"

ImageFragments::ImageFragments(PixelCalculationThread* fragment)
{
    table = fragment->GetCopyofData(&sizeX,&sizeY,&startX,&startY,&segmentId);
}

ImageFragments::~ImageFragments()
{
    free(table);
}

