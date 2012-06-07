#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMediaPlayer>
#include <QNetworkAccessManager>

class QBuffer;

class MediaPlayer : public QMediaPlayer
{
	Q_OBJECT
	Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
	Q_PROPERTY(quint8 bufferFillLevel READ bufferFillLevel WRITE setBufferFillLevel NOTIFY bufferFillLevelChanged)
	Q_PROPERTY(quint8 bufferLevel READ getBufferLevel NOTIFY bufferLevelChanged)

public:
	explicit MediaPlayer(QObject *parent = 0);
	
	QString source() { return mSource; }
	void setSource(QString src);

	quint8 bufferFillLevel() { return mBufferFillLevel; }
	void setBufferFillLevel(quint8 fillLevel) { mBufferFillLevel = fillLevel; }

	quint8 getBufferLevel() { return bufferLevel;}

signals:
	void sourceChanged();
	void bufferFillLevelChanged();
	void bufferLevelChanged();

public slots:

	void playManual();
	void pauseManual();

private slots:
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void readyRead();
	void finished();

private: //methods
	void playAuto();
	void pauseAuto();

private:
	QBuffer* stream;
	QString mSource;
	QNetworkAccessManager* nm;
	quint8 mBufferFillLevel;
	quint8 bufferLevel;

	bool isPlayManual;
	bool isPauseManual;
	bool isStopManual;
};

#endif // MEDIAPLAYER_H
