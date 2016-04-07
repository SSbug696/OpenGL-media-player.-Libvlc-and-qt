#include "EventsEmitter.h"
#include "Mediaplayer.h"
#include <QList>
#include <iostream>

void callbackEvents(const libvlc_event_t *, void *);
EventsEmitter * e_emitter;

EventsEmitter::EventsEmitter(QObject *parent) : QObject(parent)
{}


EventsEmitter::EventsEmitter(Interface * interface) {
    _interface = interface;
    e_emitter = this;
    events = libvlc_media_player_event_manager(_interface->ptrP->mediaPlayer->getInstance());
    bindEvents();
}


void EventsEmitter::bindEvents() {
    this->connect(this, SIGNAL(Playing()), _interface, SLOT(updateStatusControlPanel()));
    QList<libvlc_event_e> list;
    list << libvlc_MediaPlayerMediaChanged
         << libvlc_MediaPlayerNothingSpecial
         << libvlc_MediaPlayerOpening
         << libvlc_MediaPlayerBuffering
         << libvlc_MediaPlayerPlaying
         << libvlc_MediaPlayerPaused
         << libvlc_MediaPlayerStopped
         << libvlc_MediaPlayerForward
         << libvlc_MediaPlayerBackward
         << libvlc_MediaPlayerEndReached
         << libvlc_MediaPlayerEncounteredError
         << libvlc_MediaPlayerTimeChanged
         << libvlc_MediaPlayerPositionChanged
         << libvlc_MediaPlayerSeekableChanged
         << libvlc_MediaPlayerPausableChanged
         << libvlc_MediaPlayerTitleChanged
         << libvlc_MediaPlayerSnapshotTaken
         << libvlc_MediaPlayerLengthChanged
         << libvlc_MediaPlayerVout;

    foreach(const libvlc_event_e &event, list) {
        libvlc_event_attach(events, event, &callbackEvents, this);
    }
    // libvlc_event_attach(events, libvlc_MediaPlayerSnapshotTaken, &callbackEvents, this);
}


void EventsEmitter::unbindEvents(){}


void EventsEmitter::Vout(){
    _interface->outFinishedEvent();
}

void EventsEmitter::PositionChanged(){
          std::cout << "PositionChanged event \n" << std::endl;
}

void EventsEmitter::Buffering(){
    _interface->bufferingEvent();
}

void EventsEmitter::TimeChanged(){
    _interface->timeChangeEvent();
}


void EventsEmitter::Playing(){
    _interface->playingEvent();
}


void EventsEmitter::Opening(){
    _interface->openEvent();
}


void EventsEmitter::Stopped(){
   std::cout << "Stopped event \n" << std::endl;
}

void callbackEvents(const libvlc_event_t *e, void *data){
    MediaPlayer *core = (MediaPlayer *)data;
    std::cout << "change!" << endl;
    switch(e->type)
      {
      case libvlc_MediaPlayerMediaChanged:
          std::cout << "emit action \n" << std::endl;
          break;
      case libvlc_MediaParsedChanged:
          std::cout << "libvlc_MediaParsedChanged action \n" << std::endl;
          break;
      case libvlc_MediaPlayerOpening:
          e_emitter->Opening();
          break;
      case libvlc_MediaPlayerBuffering:
          e_emitter->Buffering();
          break;
      case libvlc_MediaPlayerPlaying:
          e_emitter->Playing();
          break;
      case libvlc_MediaPlayerPaused:
        std::cout << "MediaPlayerPaused \n" << std::endl;
          break;
      case libvlc_MediaPlayerStopped:
          e_emitter->Stopped();
          break;
      case libvlc_MediaPlayerForward:
          std::cout << "MediaPlayerForward \n" << std::endl;
          break;
      case libvlc_MediaPlayerBackward:
          std::cout << "MediaPlayerBackward \n" << std::endl;
          break;
      case libvlc_MediaPlayerEndReached:
          std::cout << "MediaPlayerEndReached \n" << std::endl;
          break;
      case libvlc_MediaPlayerEncounteredError:
          std::cout << "MediaPlayerEncounteredError \n" << std::endl;
          break;
      case libvlc_MediaPlayerTimeChanged:
          e_emitter->TimeChanged();
          break;
      case libvlc_MediaPlayerPositionChanged:
          e_emitter->PositionChanged();
          break;
      case libvlc_MediaPlayerSeekableChanged:
          std::cout << "MediaPlayerSeekableChanged \n" << std::endl;
          break;
      case libvlc_MediaPlayerPausableChanged:
          std::cout << "MediaPlayerPausableChanged \n" << std::endl;
          break;
      case libvlc_MediaPlayerTitleChanged:
          std::cout << "MediaPlayerTitleChanged \n" << std::endl;
          break;
      case libvlc_MediaPlayerSnapshotTaken:
          std::cout << "emit action \n" << std::endl;
          break;
      case libvlc_MediaPlayerLengthChanged:
          std::cout << "MediaPlayerLengthChanged \n" << std::endl;
          break;
      case libvlc_MediaPlayerVout:
          e_emitter->Vout();
          break;
      default:
          break;
      }
}
