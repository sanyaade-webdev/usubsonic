#include "testclass.h"

#include <QtDebug>

TestClass::TestClass(QObject *parent) :
	QObject(parent)
{
	subsonic = new Subsonic("http://67.189.97.36:40080");

	subsonic->setUsername("test");
	subsonic->setPassword("test");

	subsonic->getIndexes();

	connect(subsonic,SIGNAL(artistsReceived(QList<IndexFolder*>)),this,
			SLOT(artistFoldersUpdated(QList<IndexFolder*>)));

	connect(subsonic, SIGNAL(songsReceived(QList<MusicObject*>)), this,
			SLOT(songsUpdated(QList<MusicObject*>)));
}

void TestClass::artistFoldersUpdated(QList<IndexFolder *> artistFolders)
{
	Q_ASSERT(artistFolders.count());

	subsonic->getMusicDirectory(artistFolders.at(0)->id());
}

void TestClass::songsUpdated(QList<MusicObject *> songs)
{
	Q_ASSERT(songs.count());

	qDebug()<<"title: "<<songs[0]->title();
	qDebug()<<"id: "<<songs[0]->id();

	subsonic->download(songs[0],songs[0]->id() + ".mp3");
}
