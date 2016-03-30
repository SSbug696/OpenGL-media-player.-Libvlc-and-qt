#include "ViewThread.h"
#include "VideoShow.h"
#include "Player.h"
#include <iostream>


void ViewThread::nextFrame(){
    videoShow->update();
}


void ViewThread::run(){
    timer = new QTimer(this);
    //it's framerate
    timer->start(24);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
}

void ViewThread::activeAnaglyph(){
    videoShow->modeAnaglyph();
}
