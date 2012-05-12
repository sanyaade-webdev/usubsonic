#ifndef SUBSONICMEDIAPLAYER_H
#define SUBSONICMEDIAPLAYER_H

#include <QMediaPlayer>
#include <subsonic.h>

//QTM_USE_NAMESPACE

class SubsonicMediaPlayer : public QMediaPlayer
{
	Q_OBJECT
	Q_PROPERTY(QString source READ source WRITE setSource)
public:
	explicit SubsonicMediaPlayer(QObject *parent = 0);
	
	QString source() { return mSource; }
	void setSource(QString url);

signals:
	
public slots:

private:
	QString mSource;
	
};

#endif // SUBSONICMEDIAPLAYER_H
