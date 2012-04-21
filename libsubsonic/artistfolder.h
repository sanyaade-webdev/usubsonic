#ifndef ARTISTFOLDER_H
#define ARTISTFOLDER_H

#include <QObject>

class ArtistFolder : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString id READ id WRITE setId)
	Q_PROPERTY(QString name READ name WRITE setName)
public:
	explicit ArtistFolder(QObject *parent = 0);
	void setId(QString i) { mId = i; }
	QString id() { return mId; }

	void setName(QString n) { mName = n; }
	QString name() { return mName; }

private:
	QString mId;
	QString mName;
};

#endif // ARTISTFOLDER_H
