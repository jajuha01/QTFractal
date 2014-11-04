/*
** Copyright (c) 2014 Jarmo Hautakoski
**
** Fractal project
**  */

#include "customscene.h"
#include <QGraphicsSceneMoveEvent>
#include <QDebug>

CustomScene::CustomScene(MainWindow* context)
{
    window = context;
}

CustomScene::~CustomScene()
{

}

void CustomScene::MouseCurrentPos(qreal *x, qreal *y)
{
}

void CustomScene::wheelEvent(QGraphicsSceneWheelEvent *mouseEvent)
{
    int movement = mouseEvent->delta();

    qDebug() << QString::number(movement);
    if (movement > 0 )
    {
        window->ChangeSoomLevel(0.8);
    }
    else
    {
        window->ChangeSoomLevel(1.2);
    }
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF coordinates = mouseEvent->buttonDownScenePos(Qt::LeftButton);
    qreal x,y;
    x = coordinates.x();
    y = coordinates.y();
    window->ChangeOffSet(x,y);
}
// TODO Remove method
void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    QPointF coordinates = mouseEvent->scenePos();
    previousMouseX = coordinates.x();
    previousMouseY = coordinates.y();

    window->MousePosition(previousMouseX,previousMouseY);
}

