#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include <QList>
#include "pixelcalculationthread.h"
#include "imagefragments.h"
#include <qlist.h>
#include <apptimer.h>

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow, public AppTimerIf
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void RenderNewImage(bool julian_Set);
    void CopyFragmentToImage(ImageFragments* fragment,QImage* image);
    void ChangeSoomLevel(double multiplier);
    void StartCalculation(bool julian_set);
    void ReStartCalculation();
    void ChangeOffSet(qreal x, qreal y);
    void SetDefaultValues();
    void CleanThreads();
    void UpdateScreenSize();
    void MousePosition(qreal x, qreal y);
    QPointF GetComplexCoordinatesFromPixelPos(qreal x, qreal y);
    bool JulianPreviewActive();
    ~MainWindow();
    int viewWidth,viewHeight,julianWidth,CPUAmount;

private:
    Ui::MainWindow *ui;
    int next_x,next_y;
    int cluster_size;
    int clusterId;
    int ThreadsRunning;
    double pixelSize;
    QPointF JulianViewCentreCoordinates;
    QPointF ViewCentreCoordinates;
    bool reCalculationPending;
    QGraphicsScene* scene;
    QGraphicsPixmapItem* pixmapItem;
    QGraphicsPixmapItem* pixmapItemJulian;
    QThread** ThreadList;
    PixelCalculationThread** ThreadObjList;
    QList <ImageFragments*> fragmentTable;
    Effect SelectedEffect;
    double JulianpixelSize;
    bool julianset;
    int colorBoost;
    int threads_needed;
    AppTimer* appTimer;
protected:
    /* implementation for Timeout event */
    virtual void TimerEvent(void* timer);
public slots:
   void ThreadCompleted(int thread_id);
private slots:
   void on_actionSetDefaultPosition_triggered();
   void on_actionMandelProt_Fractal_triggered();
   void on_actionZ_Z_Z_Z_C_triggered();
   void on_actionJulian_SetPreview_triggered();
   void on_horizontalSlider_sliderMoved(int position);
};

#endif // MAINWINDOW_H
