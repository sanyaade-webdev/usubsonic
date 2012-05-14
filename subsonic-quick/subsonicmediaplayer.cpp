#include "subsonicmediaplayer.h"
#include <subsonic.h>
#include <QUrl>

SubsonicMediaPlayer::SubsonicMediaPlayer(QObject *parent) :
	QMediaPlayer(parent, QMediaPlayer::StreamPlayback)
{

}

void SubsonicMediaPlayer::setSource(QString url)
{
	setMedia(QUrl(url));
}
