#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>

#include "../libsubsonic/subsonic.h"
#include "../libsubsonic/artistfolder.h"

class TestClass : public QObject
{
	Q_OBJECT
public:
	explicit TestClass(QObject *parent = 0);
	
signals:
	
public slots:
	void artistFoldersUpdated(QList<ArtistFolder*> artistFolders);

private:
	Subsonic* subsonic;
};

#endif // TESTCLASS_H
