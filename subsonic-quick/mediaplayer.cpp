#include "mediaplayer.h"
#include <QBuffer>
#include <QFile>
#include <QNetworkReply>

MediaPlayer::MediaPlayer(QObject *parent) :
	QMediaPlayer(parent)
{
	nm = new QNetworkAccessManager(this);

	stream = new QBuffer();
	stream->open(QIODevice::ReadWrite);

	setMedia(QMediaContent(), stream);

	mBufferFillLevel = 20;

	isPlayManual = false;
	isPauseManual = false;
	isStopManual = false;
}

void MediaPlayer::setSource(QString url)
{
	QNetworkRequest request;
	request.setUrl(QUrl(url));

	QNetworkReply *reply = 0;
	reply = nm->get(request);

	/*if(stream != NULL)
	{
		stream->close();
		stream->deleteLater();
	}*/

	connect(reply, SIGNAL(finished()), this, SLOT(finished()));
	connect(reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
}

void MediaPlayer::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	bufferLevel = ( (double)bytesReceived / (double)bytesTotal ) * 100;

	bufferLevelChanged();

	quint8 progress = ( (double)position() / (double)duration() ) * 100;

	qDebug()<<"progress: "<<progress<<" buffer: "<<bufferLevel<<" buffer fill level: "<<bufferFillLevel();

	if( progress >= bufferLevel-1 && state() == QMediaPlayer::PlayingState )
	{
		pauseAuto();
	}

	if( bufferLevel >= progress + bufferFillLevel() && (( !isPauseManual &&
			(state() == QMediaPlayer::PausedState) ) ||
			state() == QMediaPlayer::StoppedState ))
	{
		qDebug()<<"trying to play media";
		playAuto();
	}
}

void MediaPlayer::readyRead()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QByteArray data = reply->readAll();

	qDebug()<<"writing "<<stream->write(data)<<" bytes";
	qDebug()<<stream->bytesAvailable()<<" bytes available to read";
}

void MediaPlayer::finished()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

	if(!possibleRedirectUrl.toUrl().isEmpty())
	{
		reply->deleteLater();
		reply = nm->get(QNetworkRequest(possibleRedirectUrl.toUrl()));

		connect(reply,SIGNAL(finished()),this,SLOT(finished()));
		connect(reply,SIGNAL(readyRead()),this,SLOT(readyRead()));
		connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));

		return;
	}

	reply->deleteLater();
}

void MediaPlayer::playAuto()
{
	isPlayManual = false;
	play();
}

void MediaPlayer::pauseAuto()
{
	isPauseManual = false;
	pause();
}

void MediaPlayer::playManual()
{
	isPlayManual = true;
	play();
}

void MediaPlayer::pauseManual()
{
	isPauseManual = true;
	pause();
}


