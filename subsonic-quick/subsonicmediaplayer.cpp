#include "subsonicmediaplayer.h"
#include <subsonic.h>
#include <QUrl>

SubsonicMediaPlayer::SubsonicMediaPlayer(QObject *parent) :
	QMediaPlayer(parent)
{

}

void SubsonicMediaPlayer::setSource(QString url)
{
	setMedia(QUrl(url));
}
