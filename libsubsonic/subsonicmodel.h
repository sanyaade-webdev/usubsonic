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

#ifndef SUBSONICMODEL_H
#define SUBSONICMODEL_H

#include <QObject>
#include <QDeclarativeListProperty>
#include <QSettings>
#include <QDir>

#include "songobject.h"
#include "artistfolder.h"
#include "subsonic.h"

class SubsonicModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QDeclarativeListProperty<IndexFolder> index READ getArtists NOTIFY indexChanged)
	Q_PROPERTY(QDeclarativeListProperty<MusicObject> songs READ getSongs NOTIFY songsChanged)
	Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl)
	Q_PROPERTY(QString username READ username WRITE setUsername)
	Q_PROPERTY(QString password READ password WRITE setPassword)
	Q_PROPERTY(int bufferProgress READ buffer NOTIFY bufferChanged)
	Q_PROPERTY(bool songDownloaded READ songDownloaded NOTIFY songDownloadedChanged)

public:
	explicit SubsonicModel(QObject *parent = 0);

	QDeclarativeListProperty<IndexFolder> getArtists()
	{
		return QDeclarativeListProperty<IndexFolder>(this,mArtists);
	}

	QDeclarativeListProperty<MusicObject> getSongs()
	{
		return QDeclarativeListProperty<MusicObject>(this,mSongs);
	}

	QString serverUrl()
	{
		QSettings settings(QDir::homePath()+"/.usubsonic", QSettings::IniFormat);
		return settings.value("serverUrl").toString();
	}
	void setServerUrl(QString v)
	{
		QSettings settings(QDir::homePath()+"/.usubsonic", QSettings::IniFormat);
		return settings.setValue("serverUrl",v);
	}

	QString username()
	{
		QSettings settings(QDir::homePath()+"/.usubsonic", QSettings::IniFormat);
		return settings.value("username").toString();
	}
	void setUsername(QString v)
	{
		QSettings settings(QDir::homePath()+"/.usubsonic", QSettings::IniFormat);
		settings.setValue("username",v);
	}

	QString password()
	{
		QSettings settings(QDir::homePath()+"/.usubsonic", QSettings::IniFormat);
		return settings.value("password").toString();
	}
	void setPassword(QString v)
	{
		QSettings settings(QDir::homePath()+"/.usubsonic", QSettings::IniFormat);
		settings.setValue("password",v);
	}

	int buffer() { return subsonic ? subsonic->getBufferProgress():0; }
	bool songDownloaded() { return mSongDownloaded; }

signals:
	void songsChanged();
	void indexChanged();
	void bufferChanged(int);
	void songDownloadedChanged();

public slots:
	void connect();
	QString bufferSong(MusicObject* song);
	void getRandomSongs(int num=10, QString genre="", QString fromYear="", QString toYear="", QString musicFolderId="");
	void getMusicObjectsForId(QString id);

	QList<MusicObject*> songs() { return mSongs; }
	QList<IndexFolder*> artists() { return mArtists; }

	QString streamUrl(MusicObject* song);
	QString albumArtUrl(MusicObject* song);

	//void refresh();

private slots:

	void artistsReceived(QList<IndexFolder*> list);
	void songsReceived(QList<MusicObject*> list);
    void garbageCollect();

private:
	Subsonic* subsonic;
	QList<MusicObject*> mSongs;
	QList<IndexFolder*> mArtists;
    QList<QObject*> garbage;

	bool mSongDownloaded;
};

#endif // SUBSONICMODEL_H
