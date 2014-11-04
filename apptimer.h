#ifndef APPTIMER_H
#define APPTIMER_H

#include <QtCore>

class AppTimerIf
{
public:
    virtual void TimerEvent (void* timer) = 0;

};

class AppTimer : public QObject
{
    Q_OBJECT
public:
    AppTimer();
    ~AppTimer();
    AppTimer(AppTimerIf* callback);
    void SetTimeout(int timeout);
    int TimeOutCount();
    void Stop();
    void Continue();
    QTimer* timer;
private:
    AppTimerIf* event;
    int time_out;
    int time_out_count;
public slots:
    void MySlot();
};



#endif // APPTIMER_H
