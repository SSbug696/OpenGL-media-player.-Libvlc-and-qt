#include "DelegateControl.h"
#include "Mediaplayer.h"

/*
DelegateControl::DelegateControl(QObject *parent) : QObject(parent)
{}*/

DelegateControl::DelegateControl(Mediaplayer * mp){
    _mp = mp;
}


void DelegateControl::play(){
    //_mp->play();
}


void DelegateControl::pause(){
    //_mp->pause();
}

