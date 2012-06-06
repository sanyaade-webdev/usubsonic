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
};

#endif // SUBSONIC_H
