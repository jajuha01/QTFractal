/*
** Copyright (c) 2014 Jarmo Hautakoski
**
** Fractal project
**  */

#ifndef PIXELCALCULATIONTHREAD_H
#define PIXELCALCULATIONTHREAD_H

#include <QThread>
#include <QSize>
#include <QPointF>
#include <complex>


#define ITERATION_MAX_AMOUNT 1024
#define ITERATION_STOPPED ITERATION_MAX_AMOUNT*2

enum Effect {
    MandelProt = 1,     /* F(Z) = Z^2 + C */
    Pow3PlusC = 2,       /* F(Z) = Z^3 + C*/
    JulianSetEffect = 3
};

class PixelCalculationThread : public QObject
{
    Q_OBJECT
public:
    PixelCalculationThread(int startx, int starty,int sizeOfCluster,double scale,QSize* size_value,QPointF* centre_pos,Effect value,int ColorBoost);
    ~PixelCalculationThread();
    void setup(QThread* thread,int thread_id);
    void ReInitialize(int startx, int starty,int sizeOfCluster,int cluster_id);
    unsigned int* GetCopyofData(int* size_x, int* size_y,int* start_x, int* start_y,int* cluster_id);
    Effect GetRenderEffect();
    static inline int MandelProtFractal(double orig_x, double orig_y,int iteration_max)
    {
        register int index;
        register int iterations = 0;
        register double x = orig_x;
        register double y = orig_y;
        register double xsqr = x * x;
        register double ysqr = y * y;

        /* Calculate iterations */
        for (index = 0;index < iteration_max;index++)
        {
            /* Check is values escaping sqr(z*z) < 2 -> x²+y² < 4 */
            if (xsqr + ysqr > 4)
            {
                iterations = index;
                index = ITERATION_STOPPED;
            }
            // Z^2 -> x = x²- y² , y= 2xy
            y = x * y * 2.0 + orig_y;
            x = xsqr - ysqr + orig_x;
            xsqr = x * x;
            ysqr = y * y;
        }
        return iterations;
    }
    static inline int Pow3PlusCFunc(double orig_x, double orig_y,int iteration_max)
    {
        register int index;
        register int iterations = 0;
        register double x = orig_x;
        register double y = orig_y;
        register double xsqr = x * x;
        register double ysqr = y * y;

        /* Calculate iterations */
        for (index = 0;index < iteration_max;index++)
        {
            /* Check is values escaping sqr(z*z) < 2 -> x²+y² < 4 */
            if (xsqr + ysqr > 4)
            {
                iterations = index;
                index = ITERATION_STOPPED;
            }
            // Z^3
            y = x * y * 2.0;
            x = xsqr - ysqr;
            xsqr = x * x;
            ysqr = y * y;
            y = x * y * 2.0 + orig_y;
            x = xsqr - ysqr + orig_x;
            xsqr = x * x;
            ysqr = y * y;
        }
        return iterations;
    }
    static inline int Pow3PlusCFunc2(std::complex<double> value,double orig_x, double orig_y,int iteration_max)
    {
        register int index;
        register int iterations = 0;
        /* Calculate iterations */
        for (index = 0;index < iteration_max;index++)
        {
            /* Check is values escaping sqr(z*z) < 2 -> z*z < 4 */
            if (abs(value) > 2)
            {
                iterations = index;
                index = ITERATION_STOPPED;
            }
            value = std::pow(value,3);
            value.imag() += orig_y;
            value.real() += orig_x;
        }
        return iterations;
    }
private:
    QThread* threadP;
    int size,startX,startY,ThreadId;
    double CurrentScale;
    int sizeX;
    int sizeY;
    QSize windowSize;
    QPointF centre;
    unsigned int* table;
    int table_index;
    int clusterID;
    int colorBoost;
    Effect selectedEffect;

signals:
    void CalculationDone(int id);

public slots:
    void doCalculation();
    void CalculationDone();
};

#endif // PIXELCALCULATIONTHREAD_H
