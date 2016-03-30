#include "Interface.h"
#include "Player.h"
#include "Filters.h"
#include "VideoShow.h"
#include "EventsEmitter.h"
#include "ViewThread.h"

#include "widgets/VideoSlider.h"
#include "widgets/VideoWindow.h"
#include "widgets/ControlPanel.h"
#include <QGLWidget>

#include <QMutex>
#include <QString>
#include "QTime"
#include "QDebug"
#include "QTimer"

#include "Video.h"
#include "Audio.h"
#include "Media.h"
#include <vlc/vlc.h>
#include <unistd.h>

#include <vlc/libvlc.h>
#include <vlc/libvlc_media.h>
#include <vlc/libvlc_events.h>

#include <iostream>

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_media_player_t;
struct libvlc_instance_t;
struct libvlc_new;


QTimer *timer;
QMutex mutex;

/*!
 * \brief Interface::Interface
 * \param mp
 */
Interface::Interface(Player * mp)
{
    //_videoSlider =  new VideoSlider(mp);
    _videoWindow =  new VideoWindow(mp);
    _controlPanel = new ControlPanel(mp);

    // parent->setWindowState(Qt::WindowFullScreen);

    windowW=800;
    windowH=620;

    _winID = _videoWindow->videoWindow->winId();

    view = new VideoShow(mp);

    showMediaThread = new ViewThread(view);
    thread = new QThread();
    showMediaThread->moveToThread(thread);
    thread->start();
    showMediaThread->run();
    _is_pause_event = false;
    _time_stopped   = 0;



    scaleX = (double)_videoWindow->videoWindow->width()/(double)windowW;
    scaleY = (double)_videoWindow->videoWindow->height()/(double)windowH;


    ptrP = mp;
    this->connect(_controlPanel->ratio, SIGNAL(clicked()), this, SLOT(changeRatio()));
    this->connect(_controlPanel->mute, SIGNAL(clicked()), this, SLOT(mute()));
    this->connect(_controlPanel->open, SIGNAL(pressed()), this, SLOT(openLocal()));
    this->connect(_controlPanel->play, SIGNAL(pressed()), this, SLOT(play()));
    this->connect(_controlPanel->full, SIGNAL(pressed()), this, SLOT(fullscreen()));
    this->connect(_controlPanel->pause, SIGNAL(clicked()), this, SLOT(pause()));
    this->connect(_controlPanel->anaglyph, SIGNAL(clicked()), showMediaThread, SLOT(activeAnaglyph()));



    this->connect(_controlPanel->volumeLevelSlider, SIGNAL(sliderMoved(int)), this, SLOT(valueChange(int)));
    this->connect(_controlPanel->volumeLevelSlider, SIGNAL(valueChanged(int)), this, SLOT(valueChange(int)));
    this->connect(_controlPanel->progressSlider, SIGNAL(sliderReleased()), this, SLOT(trackSlider()));
    this->connect(_controlPanel->anaglyph, SIGNAL(pressed()), this, SLOT(enabled3d()));
    this->connect(mp, SIGNAL(emitResizeEvent(QResizeEvent *)), this, SLOT(resizeEvent(QResizeEvent *)));
}


void Interface::changeRatio(){
    view->changeRatio();
}


/*!
 * \brief Interface::mute
 */
void Interface::mute(){
     ptrP->mediaPlayer->audio()->toogleMute();
}


void Interface::triggerSliderMove(){
         qDebug() << "event!!! \n";

}


void Interface::openEvent(){
          std::cout << "open event \n" << std::endl;

}

void Interface::fullscreen(){
    ptrP->setWindowState(Qt::WindowFullScreen);
    //ptrP->setWindowState( Qt::WindowNoState);
}


void Interface::play(){
    if(_is_pause_event){
        // Here you can update params render video.
        //ptrP->instance = new Instance(ptrP->filters->getArgv());
        ptrP->mediaPlayer = new MediaPlayer(ptrP->instance);
        ptrP->mediaPlayer->setPosition(_time_stopped);
        ptrP->media = new Media(ptrP->instance, _source_path);
        ptrP->mediaPlayer = new MediaPlayer(ptrP->instance);
        ptrP->mediaPlayer->open(ptrP->media,  _videoWindow->videoWindow->winId(), _source_path, view, false);


        // Bind new play event
        this->connect(_controlPanel->play, SIGNAL(released()), ptrP->mediaPlayer, SLOT(play()));

        libvlc_media_player_navigate(ptrP->mediaPlayer->_mp, _time_stopped);
        libvlc_media_player_set_position(ptrP->mediaPlayer->_mp, _time_stopped);
        libvlc_media_player_set_time(ptrP->mediaPlayer->_mp, _time_stopped);
    }
}

void Interface::playEvent(){
    qDebug() << "play event \n";
}


void Interface::playingEvent(){
    qDebug() << "playing event \n";
}


void Interface::stopEvent(){
    qDebug() << "stop event \n";
}


void Interface::bufferingEvent(){
    qDebug() <<"buffering event \n";
}


void Interface::outFinishedEvent(){
    qDebug() << "outFinished event \n";
}


void Interface::timeChangeEvent(){
    getLastTime();
}

void Interface::enabled3d(){

    //ptrP->filters->toogleFilter(Filters::P_ANAGLYPH_FILTER);
}


void Interface::updateStatusControlPanel(){
    timer = new QTimer(this);
    std::cout << "EMIT success!" << std::endl;

    int length_track     = ptrP->mediaPlayer->length();
    int current_progress =  ptrP->mediaPlayer->getTime();

    int h = ((length_track/1000)/60)/60;
    int m = (length_track/1000)/60;
    int s =  length_track/1000;
    int d = 0;
    if(s>60){
        d=m*60;
        s-=d;
    }

    QString str;
    str.append(QString::number(m));
    str+=':';
    if(s<10){
        str+='0';
    }
    str.append(QString::number(s));


    _controlPanel->progressSlider->setMaximum(length_track);
    _controlPanel->progressSlider->setRange(0, length_track);
    _controlPanel->startTime->setText(str);
}


/*!
 * \brief Interface::openLocal
 */
void Interface::openLocal(){

    const QString tr="Open file";
    QString file =
              QFileDialog::getOpenFileName(ptrP->widget, tr , QDir::homePath(), QString("Multimedia files(*)"));

      if (file.isEmpty())
          return;

      ptrP->mediaPlayer->release();
      ptrP->mediaPlayer = new MediaPlayer(ptrP->instance);
      ptrP->media = new Media(ptrP->instance, file);
      ptrP->mediaPlayer->open(ptrP->media, _winID, file, view, true);
      ptrP->eventManager = new EventsEmitter(this);
      _is_pause_event = false;
}


void Interface::getLastTime(){
    int length_track = ptrP->mediaPlayer->length();

    std::cout << length_track << std::endl;
    _controlPanel->progressSlider->setRange(0, length_track);

    int current_progress =  ptrP->mediaPlayer->getTime();

    if(!_controlPanel->volumeLevelSlider->hasTracking()){
        _controlPanel->progressSlider->setValue(current_progress);
    }

    int h = ((current_progress/1000)/60)/60;
    int m = (current_progress/1000)/60;
    int s =  current_progress/1000;
    int d = 0;
    if(s>60){
        d=m*60;
        s-=d;
    }

    QString str;
    str.append(QString::number(m));
    str+=':';
    if(s<10){
        str+='0';
    }
    str.append(QString::number(s));
    _controlPanel->startTime->setText(str);
}


void Interface::resizeEvent(QResizeEvent * event){
  int vWidth = ptrP->width()*scaleX,
      vHeight =  ptrP->height()*scaleY;

  view->setFixedWidth(vWidth);
  view->setFixedHeight(vHeight);

  int   coef_up_w = ptrP->width()/600,
        coef_up_h = ptrP->height()/500;

  _controlPanel->widget->setGeometry(
              QRect(
                  (ptrP->width() * 0.07 ) * coef_up_w,
                  (ptrP->height() * 0.8 ) * coef_up_h,
                  ptrP->width() * 0.9,
                  100
                  )
              );

}


/*!
 * \brief Interface::valueChange
 */
void Interface::valueChange(int currentVolume)
{
    ptrP->mediaPlayer->audio()->setVolume(currentVolume);
}

/*!
 * \brief Interface::trackSlider
 */
void Interface::trackSlider()
{
    float value = _controlPanel->progressSlider->value();
    ptrP->mediaPlayer->setPosition(value);
}


void Interface::eventsEmitter(QString event="default"){
}

// Pause and flush data from memory
void Interface::pause(){
    // Get current time
    _time_stopped=ptrP->mediaPlayer->getTime();
    // Add 100ms for remove time event. It's just simple hack =)
    _time_stopped += 100;
    // Get path to stopped movie
    _source_path = ptrP->media->_locationSource;
    // Disconnect play event if media-player class and remove him
    this->disconnect(_controlPanel->play, SIGNAL(released()), ptrP->mediaPlayer, SLOT(play()));
    ptrP->mediaPlayer->release();

    _is_pause_event = true;
}

