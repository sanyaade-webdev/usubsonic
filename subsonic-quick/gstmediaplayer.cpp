#include "gstmediaplayer.h"
#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/ElementFactory>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Init>
#include <QGst/Pad>
#include <QGst/GhostPad>
#include <QGst/Query>

#include <QApplication>
#include <QMetaMethod>

GstMediaPlayer::GstMediaPlayer(QObject *parent) :
	QObject(parent)
{
	mBufferFillLevel = 0;
	bufferLevel = 0;
	mPosition = 0;
	mDuration = 0;
	QMetaObject::invokeMethod(this, "positionChanged", Qt::QueuedConnection);
	QMetaObject::invokeMethod(this, "durationChanged", Qt::QueuedConnection);
	manualPause = false;

	int argc = QApplication::argc();
	char** argv =QApplication::argv();
	QGst::init(&argc,&argv);

	mSource = NULL;

	timer.setInterval(1000);

	connect(&timer,SIGNAL(timeout()),this,SLOT(checkTime()));

	mPipeline = QGst::ElementFactory::make("playbin2").dynamicCast<QGst::Pipeline>();
	if(mPipeline)
	{

		QGst::BinPtr myBin = QGst::Bin::create("myBin");

		//
		QGst::ElementPtr myEqualizer = QGst::ElementFactory::make("equalizer-10bands");
		if(!myEqualizer)
		{
			qFatal("Failed to create equalizer-10bands");
		}

		QGst::PadPtr padEqSink = myEqualizer->getStaticPad("sink");
		QGst::PadPtr padEqSrc = myEqualizer->getStaticPad("src");

		QGst::ElementPtr myVolume = QGst::ElementFactory::make("volume");
		if(!myVolume)
		{
			qFatal("Failed to create volume");
		}

		myVolume->setProperty("volume", 0.5);
		QGst::PadPtr padVolSink = myVolume->getStaticPad("sink");
		QGst::PadPtr padVolSrc = myVolume->getStaticPad("src");

		QGst::ElementPtr myAutoAudioSink = QGst::ElementFactory::make("autoaudiosink");
		if(!myAutoAudioSink)
		{
			qFatal("Failed to create autoaudiosink");
		}
		QGst::PadPtr padAudioSink = myAutoAudioSink->getStaticPad("sink");

		myBin->add(myEqualizer);
		myBin->add(myVolume);
		myBin->add(myAutoAudioSink);

		QGst::GhostPadPtr ghostSinkPad = QGst::GhostPad::create(padEqSink);
		myBin->addPad(ghostSinkPad);

		padEqSrc->link(padVolSink);
		padVolSrc->link(padAudioSink);

		mPipeline->setProperty("audio-sink", myBin);

		QGst::BusPtr bus = mPipeline->bus();
		bus->addSignalWatch();
		QGlib::connect(bus,"message",this, &GstMediaPlayer::onBusMessage);

	}
	else
	{
		qCritical()<<"Failed to create pipeline";
	}
}

void GstMediaPlayer::setSource(MusicObject* src)
{

	stop();

	if(mSource) mSource->deleteLater();

	mSource = new MusicObject(src);

	mDuration = mSource->duration() * 1000;
	durationChanged();

	mPosition = 0;
	positionChanged();

	if(!m_subsonicModel)
	{
		qCritical()<<"subsonicModel property not set!";
		return;
	}

	QString uri = m_subsonicModel->streamUrl(src);

	qDebug()<<"URI to be played: "<<uri;

	if(mPipeline)
	{
		mPipeline->setProperty("uri", uri);

		quint64 level = (double) (mSource->duration() * 1000000000) * ((double)mBufferFillLevel / 100);
		mPipeline->setProperty("buffer-duration", level);
	}
}

void GstMediaPlayer::setBufferFillLevel(int fillLevel)
{
	mBufferFillLevel = fillLevel;

}

void GstMediaPlayer::onBusMessage(const QGst::MessagePtr &message)
{
	if(message->type() == QGst::MessageEos)
	{
		//stop();
		mMediaState = EndOfMedia;
		mediaStateChanged();

	}
	else if(message->type() == QGst::MessageBuffering)
	{
		if(mMediaState != Buffering)
		{
			mMediaState = Buffering;
			mediaStateChanged();
		}

		QGst::BufferingMessagePtr buffMsg = message.staticCast<QGst::BufferingMessage>();

		qint64 bufferDuration = mPipeline->property("buffer-duration").toInt64();

		qDebug()<<"buffer percent: "<<buffMsg->percent()<<" of "<<bufferDuration;

		if(buffMsg->percent() < 100 && mPlayerState == Playing)
		{
			qDebug()<<"buffer is < 100, pausing";
			mPipeline->setState(QGst::StatePaused);
		}

		if( buffMsg->percent() == 100 && mPlayerState == Paused && !manualPause )
		{
			qDebug()<<"buffer is full, playing";
			mPipeline->setState(QGst::StatePlaying);
		}

		bufferLevel = buffMsg->percent();
		bufferLevelChanged();

	}
	else if(message->type() == QGst::MessageDuration)
	{
		//mDuration = message.staticCast<QGst::DurationMessage>()->duration() / 1000000;
		qDebug()<<"message duration: "<<mDuration;
		//durationChanged();

	}
	else if(message->type() == QGst::MessageSegmentDone)
	{
		qDebug()<<"message segment done: "<<message.staticCast<QGst::SegmentDoneMessage>()->position();
	}
	else if(message->type() == QGst::MessageStepDone)
	{
		qDebug()<<"message step done: "<<message.staticCast<QGst::StepDoneMessage>()->amount();
	}
	else if(message->type() == QGst::MessageStateChanged)
	{
		QGst::State state = message.staticCast<QGst::StateChangedMessage>()->newState();
		qDebug()<<"message state changed: "<<state;

		if(state == QGst::StatePlaying)
		{
			manualPause = false;
			if(mPlayerState != Playing)
			{
				timer.start();
				mPlayerState = Playing;
				playerStateChanged();
			}
		}
		else if(state == QGst::StatePaused)
		{
			if(mPlayerState != Paused)
			{
				timer.stop();
				mPlayerState = Paused;
				playerStateChanged();
			}

			if(!manualPause && bufferLevel == 100)
			{
				mPipeline->setState(QGst::StatePlaying);
			}
		}

		else if(state == QGst::StateNull)
		{
			timer.stop();
			mPlayerState = Stopped;
		}
	}
	else if(message->type() == QGst::MessageError)
	{
		qDebug()<<message.staticCast<QGst::ErrorMessage>()->error();
		mMediaState = Error;
		mediaStateChanged();
		stop();
	}
	else if(message->type() == QGst::MessageTag)
	{
		qDebug()<<"Tag message";

		QGst::TagMessagePtr tag = message.staticCast<QGst::TagMessage>();

		qDebug()<<"bitrate: "<<tag->taglist().bitrate();
	}
	else
	{
		qDebug()<<"Unknown message: "<<message->type();
	}

}

void GstMediaPlayer::play()
{
	if(mPipeline)
	{
		mPipeline->setState(QGst::StatePlaying);
	}
}

void GstMediaPlayer::pause()
{
	manualPause = true;
	if(mPipeline)
	{
		mPipeline->setState(QGst::StatePaused);
	}
}

void GstMediaPlayer::stop()
{
	if(mPipeline)
	{
		mPipeline->setState(QGst::StateNull);
	}
}

void GstMediaPlayer::checkTime()
{
	QGst::PositionQueryPtr pos = QGst::PositionQuery::create(QGst::FormatTime);

	if(mPipeline->query(pos))
	{
		mPosition = pos->position() / 1000000;
		//qDebug()<<"position changed: "<<mPosition<<" of "<<mDuration;
		positionChanged();
	}
}
