/*
** Copyright (c) 2014 Jarmo Hautakoski
**
** Fractal project
**  */

#include "pixelcalculationthread.h"
#include <QDebug>
#include <complex>
#include <QRgb>
#include <QObject>

#define COLOR_PALET_SIZE 10
#define RGB_MAX_VALUE 255
#define DEFAULT_FRACTAL


PixelCalculationThread::PixelCalculationThread(int startx, int starty,int sizeOfCluster,double scale,QSize* size_value,QPointF* centre_pos,Effect value,int ColorBoost)
{
    int size_of_table;
    startX = startx;
    startY = starty;
    size = sizeOfCluster;
    CurrentScale = scale;
    windowSize = *size_value;
    centre = *centre_pos;
    selectedEffect = value;
    colorBoost = ColorBoost+1;
    /* Initialize table where calculated data is stored. This is re used in case object is restarted */
    size_of_table = ((size*size));
    table = (unsigned int *) new unsigned int[size_of_table];
}

PixelCalculationThread::~PixelCalculationThread()
{
    delete[] table;
    disconnect(threadP,SIGNAL(started()),this,SLOT(doCalculation()));
    disconnect(threadP,SIGNAL(finished()),this,SLOT(CalculationDone()));
}

void PixelCalculationThread::setup(QThread *thread,int thread_id)
{
    ThreadId = thread_id;
    threadP = thread;
    clusterID = thread_id;
    connect(thread,SIGNAL(started()),this,SLOT(doCalculation()));
    connect(thread,SIGNAL(finished()),this,SLOT(CalculationDone()));
}

void PixelCalculationThread::ReInitialize(int startx, int starty, int sizeOfCluster,int cluster_id)
{
    startX = startx;
    startY = starty;
    size = sizeOfCluster;
    clusterID = cluster_id;
}

unsigned int* PixelCalculationThread::GetCopyofData(int* size_x, int* size_y,int* start_x, int* start_y,int* cluster_id)
{
    unsigned int size_of_table = (sizeX*sizeY);

    unsigned int* newTable = (unsigned int *) new unsigned int[size_of_table];
    memcpy(newTable,table,(size_of_table*sizeof(unsigned int)));

    *size_x = sizeX;
    *size_y = sizeY;
    *start_x = startX;
    *start_y = startY;
    *cluster_id = clusterID;

    return newTable;
}

void PixelCalculationThread::doCalculation()
{
    double XinScale, YinScale,YStart;
    int iteration_max_amount = ITERATION_MAX_AMOUNT;
    register int index1,index2;
    QRgb rgb;
    table_index = 0;
    unsigned int* cursor = table;
    int iterations = 0;
    int r,g,b = 0;

    qDebug() << "Start calculation: " + QString::number(clusterID);

    /* Check  that we don't go over window boundaries */
    sizeX = size;
    sizeY = size;
    if ((startX+sizeX) > windowSize.width())
    {
        sizeX = windowSize.width()-startX;
    }
    if ((startY+sizeY) > windowSize.height())
    {
        sizeY = windowSize.height()-startY;
    }
    /* Calculate complex position where iteratios will start for this cluster */
    XinScale =  centre.x() + ((startX-(windowSize.width()/2))*CurrentScale);
    YStart = centre.y() + ((startY-(windowSize.height()/2))*CurrentScale);

    /* Calculate iterations for All pixels and use iteration amount for pixel color selection */
    for (index1 = 0;index1 < sizeX;index1++)
    {
        XinScale += CurrentScale;
        YinScale = YStart;
        for (index2 = 0;index2 < sizeY; index2++)
        {
            /* Calculate current zoom levsizeel coordinates which are used to calculate colors */
            YinScale += CurrentScale;

            if (selectedEffect == MandelProt || selectedEffect == JulianSetEffect)
            {
                iterations = MandelProtFractal(XinScale,YinScale,iteration_max_amount);
            }
            else if (selectedEffect == Pow3PlusC)
            {
                iterations = Pow3PlusCFunc(XinScale,YinScale,iteration_max_amount);
            }

            /* Color RGB calculations
             * Use given colort boost and prevent color value overflow
             * blue can overflow because it give just nice effect =) */
            b = iterations&0xff;
            b *= colorBoost;

            g = iterations&0xfff0;
            g = g >> 8;
            g = (g > RGB_MAX_VALUE) ? RGB_MAX_VALUE : g;

            r = iterations&0xfff0;
            r = r >> 8;
            r *= colorBoost;
            r = (r > RGB_MAX_VALUE) ? RGB_MAX_VALUE : r;
            /* Do needed bit shift to get unsiggned int value from rgb components */
            rgb = qRgb(r, g, b);
            *cursor = rgb;
            cursor++;
        }
    }
    threadP->quit();
}

void PixelCalculationThread::CalculationDone()
{
    qDebug() << "Calculation done: " + QString::number(clusterID);
    emit CalculationDone(ThreadId);
}

Effect PixelCalculationThread::GetRenderEffect()
{
    return selectedEffect;
}
