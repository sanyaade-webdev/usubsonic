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
	Q_PROPERTY(QString album READ album WRITE setAlbum)
	AUTOPROPERTY(QString, album, Album)
	Q_PROPERTY(QString artist READ artist WRITE setArtist)
	AUTOPROPERTY(QString, artist, Artist)
	Q_PROPERTY(int bitRate READ bitRate WRITE setBitRate )
	AUTOPROPERTY(int, bitRate, BitRate)
	Q_PROPERTY(QString contentType READ contentType WRITE setContentType )
	AUTOPROPERTY(QString, contentType, ContentType)
	Q_PROPERTY(QString coverArt READ coverArt WRITE setCoverArt )
	AUTOPROPERTY(QString, coverArt, CoverArt)
	Q_PROPERTY(quint64 duration READ duration WRITE setDuration )
	AUTOPROPERTY(quint64, duration, Duration)
	Q_PROPERTY(QString genre READ genre WRITE setGenre )
	AUTOPROPERTY(QString, genre, Genre)
	Q_PROPERTY(QString id READ id WRITE setId )
	AUTOPROPERTY(QString, id, Id)
	Q_PROPERTY(bool isDir READ isDir WRITE setIsDir )
	AUTOPROPERTY(bool, isDir, IsDir)
	Q_PROPERTY(bool isVideo READ isVideo WRITE setIsVideo )
	AUTOPROPERTY(bool, isVideo, IsVideo)
	Q_PROPERTY(QString path READ path WRITE setPath )
	AUTOPROPERTY(QString, path, Path)
	Q_PROPERTY(quint64 size READ size WRITE setSize )
	AUTOPROPERTY(quint64, size, Size)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix )
	AUTOPROPERTY(QString, suffix, Suffix)
	Q_PROPERTY(QString title READ title WRITE setTitle )
	AUTOPROPERTY(QString, title, Title)
	Q_PROPERTY(int track READ track WRITE setTrack )
	AUTOPROPERTY(int, track, Track)
	Q_PROPERTY(QString year READ year WRITE setYear )
	AUTOPROPERTY(QString, year, Year)

public:
	explicit MusicObject(QObject *parent = 0);
	MusicObject(MusicObject* other);

	
};

#endif // SONGOBJECT_H

