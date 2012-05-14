#ifndef SONGOBJECT_H
#define SONGOBJECT_H

#include <QObject>

#define AUTOPROPERTY(type, name, Name) \
public: \
	void set ## Name(type s) { m_ ## name = s; } \
type name() { return m_ ## name; } \
	private: \
type m_ ## name;

class MusicObject : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY changed)
	AUTOPROPERTY(QString, album, Album)
	Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY changed)
	AUTOPROPERTY(QString, artist, Artist)
	Q_PROPERTY(int bitRate READ bitRate WRITE setBitRate NOTIFY changed)
	AUTOPROPERTY(int, bitRate, BitRate)
	Q_PROPERTY(QString contentType READ contentType WRITE setContentType NOTIFY changed)
	AUTOPROPERTY(QString, contentType, ContentType)
	Q_PROPERTY(QString coverArt READ coverArt WRITE setCoverArt NOTIFY changed)
	AUTOPROPERTY(QString, coverArt, CoverArt)
	Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY changed)
	AUTOPROPERTY(int, duration, Duration)
	Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY changed)
	AUTOPROPERTY(QString, genre, Genre)
	Q_PROPERTY(QString id READ id WRITE setId NOTIFY changed)
	AUTOPROPERTY(QString, id, Id)
	Q_PROPERTY(bool isDir READ isDir WRITE setIsDir NOTIFY changed)
	AUTOPROPERTY(bool, isDir, IsDir)
	Q_PROPERTY(bool isVideo READ isVideo WRITE setIsVideo NOTIFY changed)
	AUTOPROPERTY(bool, isVideo, IsVideo)
	Q_PROPERTY(QString path READ path WRITE setPath NOTIFY changed)
	AUTOPROPERTY(QString, path, Path)
	Q_PROPERTY(quint64 size READ size WRITE setSize NOTIFY changed)
	AUTOPROPERTY(quint64, size, Size)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix NOTIFY changed)
	AUTOPROPERTY(QString, suffix, Suffix)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY changed)
	AUTOPROPERTY(QString, title, Title)
	Q_PROPERTY(int track READ track WRITE setTrack NOTIFY changed)
	AUTOPROPERTY(int, track, Track)
	Q_PROPERTY(QString year READ year WRITE setYear NOTIFY changed)
	AUTOPROPERTY(QString, year, Year)

public:
	explicit MusicObject(QObject *parent = 0);

signals:
	void changed();

public slots:
	
};

#endif // SONGOBJECT_H

