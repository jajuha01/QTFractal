#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pixelcalculationthread.h"
#include <QImage>
#include <QDebug>
#include <customscene.h>
#include <QGraphicsPixmapItem>
#include "apptimer.h"

#define DEFAULT_PIXEL_SIZE 0.01
#define DEFAULT_SCREEN_SIZE_W 600
#define DEFAULT_SCREEN_SIZE_H 400
#define JULIANT_SET_PREVIEW_SIZE
#define DEFAULT_COLOR_BOOST 10

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new CustomScene(this);
    ui->graphicsView->setScene(scene);
    /* Enable mouse move events and track those via Scene to allow get scene position */
    ui->graphicsView->setMouseTracking(true);
    SetDefaultValues();
    pixmapItem = NULL;
    pixmapItemJulian = NULL;
    SelectedEffect = MandelProt;
    /* Not used currently
    appTimer = new AppTimer((AppTimerIf*)this);
    appTimer->SetTimeout(200);*/
    StartCalculation(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    CleanThreads();
    scene->removeItem((QGraphicsItem*)pixmapItem);
    delete pixmapItem;
    delete scene;
}

void MainWindow::RenderNewImage(bool julian_Set)
{
    QSize size(viewWidth,viewHeight);

    if (julian_Set)
    {
        size.setWidth(julianWidth);
    }
    QImage* image = new QImage(size,QImage::Format_RGB32);
    /* Remove possible previous Pixmap object */

    QPixmap pixmap(size);

    /* Combine image object from fragments and clean fragment list */
    while (fragmentTable.size())
    {
        CopyFragmentToImage(fragmentTable.at(0),image);
        delete fragmentTable.at(0);
        fragmentTable.removeAt(0);
    }


    pixmap.convertFromImage(*image);
    delete image;
    if (julian_Set)
    {
        if (pixmapItemJulian != NULL)
        {
            scene->removeItem((QGraphicsItem*)pixmapItemJulian);
            delete pixmapItemJulian;
        }
        pixmapItemJulian = scene->addPixmap(pixmap);
        /* Set pitmap to start from end of default view */
        pixmapItemJulian->moveBy(viewWidth,0);
    }
    else
    {
        if (pixmapItem != NULL)
        {
            scene->removeItem((QGraphicsItem*)pixmapItem);
            delete pixmapItem;
        }
        pixmapItem = scene->addPixmap(pixmap);
     }
}

void MainWindow::CopyFragmentToImage(ImageFragments *fragment,QImage* image)
{
    int index1,index2 = 0;
    unsigned int* cursor = fragment->table;

    for (index1 = 0;index1 < fragment->sizeX;index1++)
    {
        for (index2 = 0;index2 < fragment->sizeY; index2++)
        {
        image->setPixel((fragment->startX+index1),(fragment->startY+index2),*cursor);
        cursor++;
        }
    }
}

void MainWindow::ChangeSoomLevel(double multiplier)
{
    if (julianset)
    {
        JulianpixelSize *= multiplier;
    }
    else
    {
        pixelSize *= multiplier;
        /* TODO: Leave this out need finetuning to work properly
        if (multiplier > 1)
        {
            scale = pixmapItem->scale();
            pixmapItem->setScale((qreal)(scale*multiplier));
        } */
    }
    /* Leave this out need finetuning to work properly
    if (scale > 2 || scale < 1)
    {*/
    ReStartCalculation();
    //}
}

void MainWindow::StartCalculation(bool julian_set)
{
    int i;
    QSize size(viewWidth,viewHeight);
    QPointF coordinates = ViewCentreCoordinates;
    double pixel_size = pixelSize;
    Effect effect = SelectedEffect;
    /* Clean possible old threads */
    CleanThreads();
    /* Start Threading */
    CPUAmount = QThread::idealThreadCount();
    ThreadList = new QThread*[CPUAmount];
    ThreadObjList = new PixelCalculationThread*[CPUAmount];

    /* In case julian preview mode then tune few parameters */
    if (julian_set)
    {
        size.setWidth(julianWidth);
        effect = JulianSetEffect;
        coordinates = JulianViewCentreCoordinates;
        pixel_size = JulianpixelSize;
    }

    /* Calculate optimal cluster size */
    cluster_size = size.width()/CPUAmount;

    /* Initialize where first completed thread calculation should start */
    next_x = 0;
    next_y = cluster_size;
    ThreadsRunning = 0;
    /* Create threads and start calculate first vawe of pitmap data */

    for (i= 0; i < CPUAmount;i++)
    {

        ThreadObjList[i] = new PixelCalculationThread(cluster_size*i,0,cluster_size,pixel_size,&size,&coordinates,effect,colorBoost);
        ThreadList[i] = new QThread();
        ThreadObjList[i]->setup(ThreadList[i],i);
        ThreadObjList[i]->moveToThread(ThreadList[i]);
        connect(ThreadObjList[i],SIGNAL(CalculationDone(int)),this,SLOT(ThreadCompleted(int)));
        ThreadList[i]->start();
        ThreadsRunning++;
    }
    /* Calculate total threads needed */
    threads_needed = 0;
    threads_needed = (size.height()/cluster_size)*CPUAmount;
    if (size.height()%cluster_size)
    {
        threads_needed +=CPUAmount;
    }
    ui->progressBar->setValue(0);
    clusterId = CPUAmount;
}

void MainWindow::ReStartCalculation()
{
    UpdateScreenSize();

    if (ThreadsRunning == 0)
    {
        StartCalculation(julianset);
    }
    else
    {
        reCalculationPending = true;
    }
}

void MainWindow::ChangeOffSet(qreal x, qreal y)
{
    qreal current_x,current_y;
    /* Calculate how much we need to move centre coordinates */
    current_x = ViewCentreCoordinates.x();
    current_y = ViewCentreCoordinates.y();
    current_x += (x-(viewWidth/2))*pixelSize;
    ViewCentreCoordinates.setX(current_x);
    current_y += (y-(viewHeight/2))*pixelSize;
    ViewCentreCoordinates.setY(current_y);
    ReStartCalculation();
}

void MainWindow::SetDefaultValues()
{
    /* Initialize data and greate scene for view */
    viewWidth = DEFAULT_SCREEN_SIZE_W;
    viewHeight = DEFAULT_SCREEN_SIZE_H;
    pixelSize = DEFAULT_PIXEL_SIZE;
    /* Set offset to centre of image area */
    ViewCentreCoordinates.setX(0);
    ViewCentreCoordinates.setY(0);
    JulianViewCentreCoordinates.setX(0);
    JulianViewCentreCoordinates.setY(0);
    reCalculationPending = false;
    julianset = false;
    SelectedEffect = MandelProt;
    JulianpixelSize = pixelSize/10000;
    colorBoost = DEFAULT_COLOR_BOOST;
    ui->horizontalSlider->setValue(DEFAULT_COLOR_BOOST);
    ui->horizontalSlider->setMaximum(50);
}

void MainWindow::CleanThreads()
{
    if (ThreadsRunning == 0 && !ThreadList && !ThreadObjList)
    {
        for (int i= 0; i < CPUAmount;i++)
        {
            disconnect(ThreadObjList[i],SIGNAL(CalculationDone(int)),this,SLOT(ThreadCompleted(int)));
            delete ThreadObjList[i];
            delete ThreadList[i];

        }
        delete [] ThreadList;
        delete [] ThreadObjList;
        ThreadList = NULL;
        ThreadObjList = NULL;
    }

}

void MainWindow::UpdateScreenSize()
{
    /* Reduce width to allow draw julian set to right side of mandelprot */
    viewWidth = ui->graphicsView->viewport()->width();
    viewHeight = ui->graphicsView->viewport()->height();

    if (julianset)
    {
        /* Reduce width to allow draw julian set to right side of mandelprot */
        viewWidth /= 2;
        julianWidth = viewWidth;
    }

}

void MainWindow::MousePosition(qreal x, qreal y)
{
    if (julianset)
    {
        /* Map new coordinates */
        JulianViewCentreCoordinates = GetComplexCoordinatesFromPixelPos(x,y);

        if (ThreadsRunning == 0)
            {
            StartCalculation(true);
        }
    }
}

QPointF MainWindow::GetComplexCoordinatesFromPixelPos(qreal x, qreal y)
{
    QPointF coordinates;
    qreal new_x,new_y;
    new_y = y;
    new_x = x;

    /* Calculate how much coordinated deviate from centre and
     * calculate complex coordinate offset based of zoomlevel */
    new_x = ((new_x-(viewWidth/2))*pixelSize);
    new_y = ((new_y-(viewHeight/2))*pixelSize);
    coordinates.setX(ViewCentreCoordinates.x()+new_x);
    coordinates.setY(ViewCentreCoordinates.y()+new_y);
    return coordinates;
}

bool MainWindow::JulianPreviewActive()
{
    return julianset;
}



void MainWindow::ThreadCompleted(int thread_id)
{
    /* Copy data to list and store needed info to ImageFragments */
    fragmentTable.append(new ImageFragments(ThreadObjList[thread_id]));
    ThreadsRunning--;
    Effect effect = ThreadObjList[thread_id]->GetRenderEffect();
    /* Update progress bar */
    ui->progressBar->setValue(ui->progressBar->value()+(100/threads_needed));

    if (next_y < viewHeight && next_x < viewWidth)
    {
        /* Start thread again for next image cluster */
        ThreadObjList[thread_id]->ReInitialize(next_x,next_y,cluster_size,clusterId);
        ThreadList[thread_id]->start();
        ThreadsRunning++;

        /* calculate next thread initialization values */
        clusterId++;
        next_x += cluster_size;
        if (next_x >= viewWidth)
        {
            next_x = 0;
            next_y += cluster_size;
            /* cluster calculation threads will check size of window and don't go over boundaries.
             * -> no need to double check window size limits in here */
        }
    }
    else if (ThreadsRunning == 0)
    {
        qDebug() << "all calculation done";
        /* Update progress bar */
        ui->progressBar->setValue(100);
        if (effect == JulianSetEffect)
        {
            RenderNewImage(true);
        }
        else
        {
            RenderNewImage(false);
        }
        if (reCalculationPending)
        {
            ReStartCalculation();
            reCalculationPending = false;
        }
    }

}

void MainWindow::on_actionSetDefaultPosition_triggered()
{
    SetDefaultValues();
    StartCalculation(false);
}

void MainWindow::on_actionMandelProt_Fractal_triggered()
{
    SelectedEffect = MandelProt;
    julianset = false;
}

void MainWindow::on_actionZ_Z_Z_Z_C_triggered()
{
    SelectedEffect = Pow3PlusC;
    julianset = false;
}

void MainWindow::on_actionJulian_SetPreview_triggered()
{
    JulianViewCentreCoordinates.setX(0);
    JulianViewCentreCoordinates.setY(0);
    SelectedEffect = MandelProt;
    julianset = true;

    UpdateScreenSize();
    /* Calculate new image for new window size */
    StartCalculation(false);
}

void MainWindow::TimerEvent(void* timer)
{
    (void)timer;

}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    colorBoost = position;
    /* Calculate new image for new window size */
    ReStartCalculation();
}
