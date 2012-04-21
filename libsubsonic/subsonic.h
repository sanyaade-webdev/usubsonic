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
	void getMusicDirectory(ArtistFolder* folder);
	void search();
//	void search2();
//	void getPlaylists();
//	void getPlaylist();
//	void createPlaylist();
//	void deletePlaylist();
	void download(SongObject* song, QString filePath);
//	void stream();

signals:
	void artistsReceived(QList<ArtistFolder*> artists);
	void songsReceived(QList<SongObject*> songs);

private slots:
	void getMusicFoldersReply();
	void getMusicDirectoryReply();
	void getIndexesReply();
	void downloadReply();

private: //methods:
	QString urlBuilder(QString method,ArgMap args);

private:
	QNetworkAccessManager* networkAccessManager;
	QString mServer;
	QString mUsername;
	QString mPassword;
};

#endif // SUBSONIC_H
