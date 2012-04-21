#include "testclass.h"

#include <QtDebug>
#include <QGst/Buffer>

TestClass::TestClass(QObject *parent) :
	QObject(parent)
{
	subsonic = new Subsonic("http://67.189.97.36:40080");

	subsonic->setUsername("test");
	subsonic->setPassword("test");

	subsonic->getIndexes();

	connect(subsonic,SIGNAL(artistsReceived(QList<ArtistFolder*>)),this,
			SLOT(artistFoldersUpdated(QList<ArtistFolder*>)));

	connect(subsonic, SIGNAL(songsReceived(QList<SongObject*>)), this,
			SLOT(songsUpdated(QList<SongObject*>)));
}

void TestClass::artistFoldersUpdated(QList<ArtistFolder *> artistFolders)
{
	Q_ASSERT(artistFolders.count());

	subsonic->getMusicDirectory(artistFolders.at(0));
}

void TestClass::songsUpdated(QList<SongObject *> songs)
{
	Q_ASSERT(songs.count());

	qDebug()<<"title: "<<songs[0]->title();
	qDebug()<<"id: "<<songs[0]->id();

	subsonic->download(songs[0],songs[0]->id() + ".mp3");
}
