/*
** Copyright (c) 2014 Jarmo Hautakoski
**
** Fractal project
**  */

#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QGraphicsScene>
#include "mainwindow.h"

class CustomScene : public QGraphicsScene
{
public:
    CustomScene(MainWindow* context);
    ~CustomScene();
    void MouseCurrentPos(qreal* x,qreal* y);
    virtual void wheelEvent(QGraphicsSceneWheelEvent* mouseEvent);
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
private:
    MainWindow* window;
    qreal previousMouseX,previousMouseY;
};

#endif // CUSTOMSCENE_H
