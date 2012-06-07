/**************************************************************************
*   Copyright (C) 2012 Intel Corporation                                  *
*                                                                         *
*                                                                         *
*   This file is free software: you can redistribute it and/or modify     *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation, either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   It is distributed in the hope that it will be useful                  *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   If not, see <http://www.gnu.org/licenses/>.                           *
***************************************************************************/

#include "subsonicmodel.h"
#include "artistfolder.h"
#include "songobject.h"

#include <QtDebug>
#include <QTimer>
#include <QDir>

SubsonicModel::SubsonicModel(QObject *parent) :
	QObject(parent)
{
	subsonic = NULL;
	mSongDownloaded = false;
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
	QObject::connect(subsonic,SIGNAL(downloadBufferChanged(int)),this,
					 SIGNAL(bufferChanged(int)));
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
		qDebug()<<"garbage collecting";
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

QString SubsonicModel::bufferSong(MusicObject *song)
{
	QString filename = QDir::homePath() + "/Music/" + song->title() + ".mp3";

	if(!QFile::exists(filename))
	{
		if(QFile::exists(filename+".stream"))
			QDir::home().remove(filename+".stream");

		subsonic->download(song,filename);
		mSongDownloaded = false;
		return filename;
	}

	mSongDownloaded = true;
	return filename;
}
