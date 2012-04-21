#include "testclass.h"

TestClass::TestClass(QObject *parent) :
	QObject(parent)
{
	subsonic = new Subsonic("http://67.189.97.36:40080");

	subsonic->setUsername("test");
	subsonic->setPassword("test");

	subsonic->getIndexes();

	connect(subsonic,SIGNAL(artistsReceived(QList<ArtistFolder*>)),this,
			SLOT(artistFoldersUpdated(QList<ArtistFolder*>)));
}

void TestClass::artistFoldersUpdated(QList<ArtistFolder *> artistFolders)
{
	Q_ASSERT(artistFolders.count());

	subsonic->getMusicDirectory(artistFolders.at(0));


}
