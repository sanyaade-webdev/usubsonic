#include "subsonicmodel.h"
#include "artistfolder.h"
#include "songobject.h"

#include <QtDebug>
#include <QTimer>

SubsonicModel::SubsonicModel(QObject *parent) :
	QObject(parent)
{
	subsonic = NULL;
}

void SubsonicModel::connect()
{
	if(subsonic)
		delete subsonic;

	QSettings settings;

	QString url = settings.value("serverUrl").toString();
	QString user = settings.value("username").toString();
	QString pass = settings.value("password").toString();

	subsonic = new Subsonic(url);
	subsonic->setUsername(user);
	subsonic->setPassword(pass);

	subsonic->getIndexes();

	QObject::connect(subsonic,SIGNAL(artistsReceived(QList<IndexFolder*>)),this,
			SLOT(artistsReceived(QList<IndexFolder*>)));
	QObject::connect(subsonic,SIGNAL(songsReceived(QList<MusicObject*>)),this,
			SLOT(songsReceived(QList<MusicObject*>)));
}

void SubsonicModel::getRandomSongs(int num, QString genre, QString fromYear, QString toYear, QString musicFolderId)
{
	if(subsonic)
	{
		subsonic->getRandomSongs(num,genre,fromYear,toYear,musicFolderId);
	}
}

void SubsonicModel::getMusicObjectsForId(QString id)
{
	if(subsonic)
	{
		subsonic->getMusicDirectory(id);
	}
}


void SubsonicModel::artistsReceived(QList<IndexFolder *> list)
{
	qDebug()<<"setting artist list: "<<list.count();

	foreach(IndexFolder* obj, mArtists)
	{
		///clean up old songs list:
        garbage<<obj;
	}

    QTimer::singleShot(1000,this,SLOT(garbageCollect()));

	mArtists = list;
	indexChanged();
}

void SubsonicModel::songsReceived(QList<MusicObject *> list)
{
	qDebug()<<"setting songs list: "<<list.count();

	foreach(MusicObject* obj, mSongs)
	{
		///clean up old songs list:
        garbage<<obj;
	}

    QTimer::singleShot(1000,this,SLOT(garbageCollect()));

	mSongs = list;
    songsChanged();
}

void SubsonicModel::garbageCollect()
{
    foreach(QObject* obj, garbage)
    {
        ///clean up list:
        garbage.removeAll(obj);
        delete obj;
    }
}

QString SubsonicModel::streamUrl(MusicObject *song)
{
	if(subsonic)
	{
		ArgMap args;
		args["id"] = song->id();
		return subsonic->urlBuilder("stream", args);
	}

	return "";
}

QString SubsonicModel::albumArtUrl(MusicObject *song)
{
	if(subsonic)
	{
		ArgMap args;
		args["id"] = song->coverArt();
		return subsonic->urlBuilder("stream", args);
	}

	return "";
}

void SubsonicModel::refresh()
{

}
