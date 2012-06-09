#ifndef GSTMEDIAPLAYER_H
#define GSTMEDIAPLAYER_H

#include <QObject>
#include <QTimer>

#include <QGst/Pipeline>

#include "subsonicmodel.h"
#include "songobject.h"

class GstMediaPlayer : public QObject
{
	Q_OBJECT
	Q_PROPERTY(MusicObject* source READ source WRITE setSource NOTIFY sourceChanged)
	Q_PROPERTY(SubsonicModel* subsonicModel READ subsonicModel WRITE setSubsonicModel NOTIFY subsonicModelChanged)
	AUTOPROPERTY(SubsonicModel*, subsonicModel, SubsonicModel)
	Q_PROPERTY(int bufferFillLevel READ bufferFillLevel WRITE setBufferFillLevel NOTIFY bufferFillLevelChanged)
	Q_PROPERTY(int bufferLevel READ getBufferLevel NOTIFY bufferLevelChanged)
	Q_PROPERTY(MediaState mediaState READ mediaState NOTIFY mediaStateChanged)
	Q_PROPERTY(PlayerState playerState READ playerState NOTIFY playerStateChanged)
	Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
	Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
	Q_ENUMS(MediaState)
	Q_ENUMS(PlayerState)

public:

	enum MediaState {
		Error,
		Buffering,
		EndOfMedia
	};

	enum PlayerState {
		Stopped,
		Paused,
		Playing
	};

	explicit GstMediaPlayer(QObject *parent = 0);

	MusicObject* source() { return mSource; }
	void setSource(MusicObject* src);

	int bufferFillLevel() { return mBufferFillLevel; }
	void setBufferFillLevel(int fillLevel);

	int getBufferLevel() { return bufferLevel;}

	MediaState mediaState() { return mMediaState; }

	PlayerState playerState() { return mPlayerState; }

	qint64 position() { return mPosition; }
	qint64 duration() { return mDuration; }

signals:
	void sourceChanged();
	void bufferFillLevelChanged();
	void bufferLevelChanged();
	void mediaStateChanged();
	void subsonicModelChanged();
	void playerStateChanged();
	void positionChanged();
	void durationChanged();

public slots:
	void play();
	void pause();
	void stop();

private slots:
	void checkTime();

private: //methods
	void onBusMessage(const QGst::MessagePtr & message);

private:
	QGst::PipelinePtr mPipeline;
	int mBufferFillLevel;
	int bufferLevel;

	MusicObject* mSource;
	MediaState mMediaState;
	PlayerState mPlayerState;
	qint64 mPosition;
	qint64 mDuration;

	QTimer timer;

	bool manualPause;
};

#endif // GSTMEDIAPLAYER_H
