#ifndef SUBSONICMODEL_H
#define SUBSONICMODEL_H

#include <QObject>
#include <QDeclarativeListProperty>
#include <QSettings>

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
		QSettings settings;
		return settings.value("serverUrl").toString();
	}
	void setServerUrl(QString v)
	{
		QSettings settings;
		return settings.setValue("serverUrl",v);
	}

	QString username()
	{
		QSettings settings;
		return settings.value("username").toString();
	}
	void setUsername(QString v)
	{
		QSettings settings;
		settings.setValue("username",v);
	}

	QString password()
	{
		QSettings settings;
		return settings.value("password").toString();
	}
	void setPassword(QString v)
	{
		QSettings settings;
		settings.setValue("password",v);
	}

signals:
	void songsChanged();
	void indexChanged();

public slots:
	void connect();
	void getRandomSongs(int num=10, QString genre="", QString fromYear="", QString toYear="", QString musicFolderId="");
	void getMusicObjectsForId(QString id);

	QList<MusicObject*> songs() { return mSongs; }
	QList<IndexFolder*> artists() { return mArtists; }

	QString streamUrl(MusicObject* song);
	QString albumArtUrl(MusicObject* song);

	void refresh();

private slots:

	void artistsReceived(QList<IndexFolder*> list);
	void songsReceived(QList<MusicObject*> list);
    void garbageCollect();
private:
	Subsonic* subsonic;
	QList<MusicObject*> mSongs;
	QList<IndexFolder*> mArtists;
    QList<QObject*> garbage;
};

#endif // SUBSONICMODEL_H
