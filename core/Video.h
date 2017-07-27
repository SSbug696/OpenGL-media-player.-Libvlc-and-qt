#ifndef VIDEO_H
#define VIDEO_H
#include <QObject>

struct libvlc_media_list_player_t;
struct libvlc_media_player_has_vout;
struct libvlc_media_player_t;
struct libvlc_media_list_t;


/*!
 * \brief The Video class
 */
class Video : public QObject {
    Q_OBJECT
public:
    explicit Video(QObject *parent = 0);
    Video(libvlc_media_player_t *);

    void showLogo(const QString &, int, int, int);
    bool getSnapshot(const QString &);
    void setRatio(QString  &);

    std::vector<int> getTrackId() const;
    int getTrackCount() const;

signals:

public slots:
    void setPause();

private:
    libvlc_media_player_t * mp;

};

#endif // VIDEO_H
