#include "apptimer.h"

AppTimer::~AppTimer()
{
    disconnect(timer,SIGNAL(timeout()),this,SLOT(MySlot()));
}

AppTimer::AppTimer(AppTimerIf* callback)
{
    if (callback)
    {
        event = callback;

        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(MySlot()));
        time_out = 100;
        timer->start(time_out);
    }
    time_out_count = 0;
}

void AppTimer::MySlot()
{
    time_out_count++;
    event->TimerEvent(this);
}

void AppTimer::SetTimeout(int timeout)
{
    time_out = timeout;
    if (timer)
        timer->start(time_out);
}

int AppTimer::TimeOutCount()
{
    return time_out_count;
}

void AppTimer::Stop()
{
    if (timer)
        timer->stop();
}

void AppTimer::Continue()
{
    if (timer)
        timer->start(time_out);
}
