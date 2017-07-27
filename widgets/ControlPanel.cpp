#include "ControlPanel.h"
#include <QWidget>


ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent)
{

    player = parent;
    QHBoxLayout *layout = new QHBoxLayout;

    widget = new QWidget(parent);
    widget->setGeometry(QRect(20,490, 800, 90));
    widget->setAutoFillBackground(true);

    progressSlider = new QSlider(widget);
    progressSlider->setSingleStep(1);
    progressSlider->setBaseSize(QSize(620, 40));
    progressSlider->setOrientation(Qt::Horizontal);
    progressSlider->setGeometry(QRect(80, 10, 650,20));


    volumeLevelSlider = new QSlider(widget);
    volumeLevelSlider->setSingleStep(1);
    volumeLevelSlider->setOrientation(Qt::Horizontal);
    volumeLevelSlider->setBaseSize(QSize(100,10));
    volumeLevelSlider->setGeometry(QRect(600, 55, 130, 20));

    startTime= new QLabel(widget);
    startTime->setText(QString("00:00"));
    startTime->setGeometry(QRect(10, -10, 40, 50));

    finishTime= new QLabel(widget);
    finishTime->setText(QString("00:00"));
    finishTime->setGeometry(QRect(735, -10, 50, 50));


    full = new QPushButton(widget);
    full->setText(QString("[]"));
    full->setGeometry(QRect(40, 50, 40, 30));
    full->setStyleSheet(QString("background-color:0xfee;"));

    play = new QPushButton(widget);
    play->setText(QString("Play"));
    play->setGeometry(QRect(130, 50, 100, 30));
    play->setStyleSheet(QString("background-color:0xfee;"));

    pause = new QPushButton(widget);
    pause->setText(QString("Pause"));
    pause->setGeometry(QRect(235, 50, 100, 30));
    pause->setStyleSheet(QString("background-color:0xfee;"));

    ratio = new QPushButton(widget);
    ratio->setText(QString("Ratio"));
    ratio->setGeometry(QRect(340, 50, 100, 30));
    ratio->setStyleSheet(QString("background-color:0xfee;"));

    open = new QPushButton(widget);
    open->setText(QString("Select to file"));
    open->setGeometry(QRect(460, 50, 120, 30));
    open->setStyleSheet(QString("background-color:0xfee"));



    QLabel * anaglyph_desc = new QLabel(widget);
    anaglyph_desc->setText(QString("Anaglyph"));
    anaglyph_desc->setGeometry(QRect(64, 32, 75, 18));

    anaglyph = new QCheckBox(widget);
    anaglyph->setText(QString("Anaglyph"));
    anaglyph->setGeometry(QRect(85, 60, 20, 15));

    QLabel * mute_desc = new QLabel(widget);
    mute_desc->setText(QString("Mute"));
    mute_desc->setGeometry(QRect(10, 32, 35, 18));


    mute = new QCheckBox(widget);
    mute->setGeometry(QRect(20, 60, 20, 15));


    layout->addWidget(widget);
    parent->setLayout(layout);
}


