#ifndef MEDIA_H
#define MEDIA_H

#include <QObject>

struct libvlc_event_t;
struct libvlc_event_manager_t;
struct libvlc_media_t;
struct libvlc_instance_t;
struct libvlc_list_add_media;


class Instance;

/*!
 * \brief The Media class
 */
class Media : public QObject {
    Q_OBJECT
public:
     Media(Instance *, QString);
    ~Media();

    void Init(Instance *, QString &);
    libvlc_media_t * getMedia() const;
    libvlc_event_manager_t * getEventManager() const;
    int getMediaListCount() const;
    void setMediaToPlaylist(QString);
    QString _locationSource;

signals:
public slots:

private:
    libvlc_media_t * _media;
    libvlc_event_manager_t *_events;
    libvlc_instance_t * _instance;
};

#endif // MEDIA_H
