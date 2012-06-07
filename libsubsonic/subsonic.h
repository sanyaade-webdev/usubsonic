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

#ifndef SUBSONIC_H
#define SUBSONIC_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMap>
#include <QVariantList>

#include "artistfolder.h"
#include "songobject.h"

typedef QMap<QString,QString> ArgMap;

class Subsonic: public QObject{
	Q_OBJECT
	Q_PROPERTY(QString user READ username WRITE setUsername)
	Q_PROPERTY(QString pass READ pass WRITE setPassword)
	Q_PROPERTY(int bufferProgress READ getBufferProgress NOTIFY downloadBufferChanged)

public:
	Subsonic(QString server, QObject * parent = 0);

	QString username() { return mUsername; }
	QString pass() { return mPassword; }


	void setUsername(QString m) { mUsername = m; }
	void setPassword(QString p) { mPassword = p; }

	//void ping();
	//void getLicence();
	void getMusicFolders();
	//void getNowPlaying();
	void getIndexes();
	void getMusicDirectory(QString id);
	void search();
//	void search2();
//	void getPlaylists();
//	void getPlaylist();
//	void createPlaylist();
//	void deletePlaylist();
	void download(MusicObject* song, QString filePath);
	void getRandomSongs(int num=10, QString genre="",QString fromYear="", QString toYear="", QString musicFolderId="");
//	void stream();
	int getBufferProgress() { return bufferProgress; }

	QString urlBuilder(QString method,ArgMap args);

signals:
	void artistsReceived(QList<IndexFolder*> artists);
	void songsReceived(QList<MusicObject*> songs);

	void downloadBufferChanged(int progress );

private slots:
	void getMusicFoldersReply();
	void getMusicDirectoryReply();
	void getIndexesReply();
	void downloadReply();
	void downloadFinished();
	void downloadProgress(qint64,qint64);

private:
	QNetworkAccessManager* networkAccessManager;
	QString mServer;
	QString mUsername;
	QString mPassword;

	int bufferProgress;
	QString mCurrentDownloadFilename;
};

#endif // SUBSONIC_H
