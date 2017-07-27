#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QSlider>
#include <QtCore>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>


class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanel(QWidget *parent = 0);
    QWidget * player;
    QWidget * widget;

    QSlider * progressSlider;
    QSlider * volumeLevelSlider;
    QLabel * startTime;
    QLabel * finishTime;

    QPushButton * play;
    QPushButton * full;
    QPushButton * ratio;
    QPushButton * pause;
    QPushButton * open;
    QCheckBox * anaglyph;
    QCheckBox * mute;
    QVBoxLayout * box;

signals:

public slots:
};

#endif // CONTROLPANEL_H
