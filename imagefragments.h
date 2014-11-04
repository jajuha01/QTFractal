/*
** Copyright (c) 2014 Jarmo Hautakoski
**
** Fractal project
**  */

#ifndef IMAGEFRAGMENTS_H
#define IMAGEFRAGMENTS_H

#include <QImage>
#include <pixelcalculationthread.h>


class ImageFragments : public QObject
{
    Q_OBJECT
public:
    ImageFragments(PixelCalculationThread* fragment);
    ~ImageFragments();
    void CopyDataToBuffer(QImage);
    int segmentId;
    unsigned int* table;
    int sizeX;
    int sizeY;
    int startX;
    int startY;
};

#endif // IMAGEFRAGMENTS_H
