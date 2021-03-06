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

#include "subsonic.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkProxyFactory>
#include <QFile>
#include <QDir>

#include <qjson/parser.h>
#include <qjson/qobjecthelper.h>

QString Subsonic::urlBuilder(QString method, ArgMap args)
{
	QString url = "%1/rest/%2.view?u=%3&p=%4&v=1.6.0&c=libsubsonic&f=json";

	url = url.arg(mServer).arg(method).arg(mUsername).arg(mPassword);

	foreach(QString key, args.keys())
	{
		url+=QString("&%1=%2").arg(key).arg(args[key]);
	}

	qDebug()<<"built url: "<<url;

	return url;
}

Subsonic::Subsonic(QString server, QObject *parent)
	:QObject(parent), mServer(server)
{
	QNetworkProxyFactory::setUseSystemConfiguration(true);

	networkAccessManager = new QNetworkAccessManager(this);
}

void Subsonic::getMusicFolders()
{
	QString url = urlBuilder("getMusicFolders", ArgMap());

	QNetworkRequest request;
	request.setUrl(QUrl(url));

	QNetworkReply *reply = 0;
	reply = networkAccessManager->get(request);

	connect(reply,SIGNAL(finished()),this,SLOT(getMusicFoldersReply()));
}

void Subsonic::getIndexes()
{
	QString url = urlBuilder("getIndexes", ArgMap());

	QNetworkRequest request;
	request.setUrl(QUrl(url));

	QNetworkReply *reply = 0;
	reply = networkAccessManager->get(request);

	connect(reply,SIGNAL(finished()),this,SLOT(getIndexesReply()));
}

void Subsonic::getMusicDirectory(QString id)
{
	ArgMap args;
	args["id"] = id;
	QString url = urlBuilder("getMusicDirectory", args);

	QNetworkRequest request;
	request.setUrl(QUrl(url));

	QNetworkReply *reply = 0;
	reply = networkAccessManager->get(request);

	connect(reply,SIGNAL(finished()),this,SLOT(getMusicDirectoryReply()));
}

void Subsonic::getMusicFoldersReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

	qDebug()<<"is redirect? "<<possibleRedirectUrl;

	if(!possibleRedirectUrl.toUrl().isEmpty())
	{
		qDebug()<<"we got redirected to: "<<possibleRedirectUrl.toUrl().toString();
		reply->deleteLater();
		reply = networkAccessManager->get(QNetworkRequest(possibleRedirectUrl.toUrl()));

		connect(reply,SIGNAL(finished()),this,SLOT(getMusicFoldersReply()));

		return;
	}

	QString data = reply->readAll();

	qDebug()<<"reply: "<<data;

	QJson::Parser parser;
	QVariant json = parser.parse(data.toAscii());

	QVariant folders = json.toMap()["subsonic-response"].toMap()["musicFolders"].toMap()["musicFolder"];

	reply->deleteLater();
}

void Subsonic::getMusicDirectoryReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

	qDebug()<<"is redirect? "<<possibleRedirectUrl;

	if(!possibleRedirectUrl.toUrl().isEmpty())
	{
		qDebug()<<"we got redirected to: "<<possibleRedirectUrl.toUrl().toString();
		reply->deleteLater();
		reply = networkAccessManager->get(QNetworkRequest(possibleRedirectUrl.toUrl()));

		connect(reply,SIGNAL(finished()),this,SLOT(getMusicDirectoryReply()));

		return;
	}

	QString data = reply->readAll();

	qDebug()<<"reply: "<<data;

	QJson::Parser parser;
	QVariant json = parser.parse(data.toAscii());

	QVariantList songsList = json.toMap()["subsonic-response"].toMap()["directory"].toMap()["child"].toList();

	QList<MusicObject*> songs;

	foreach(QVariant song, songsList)
	{
		MusicObject* songObject = new MusicObject(this);

		QJson::QObjectHelper::qvariant2qobject(song.toMap(),songObject);
		songs<<songObject;
	}

	songsReceived(songs);

	reply->deleteLater();
}

void Subsonic::getIndexesReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

	qDebug()<<"is redirect? "<<possibleRedirectUrl;

	if(!possibleRedirectUrl.toUrl().isEmpty())
	{
		qDebug()<<"we got redirected to: "<<possibleRedirectUrl.toUrl().toString();
		reply->deleteLater();
		reply = networkAccessManager->get(QNetworkRequest(possibleRedirectUrl.toUrl()));

		connect(reply,SIGNAL(finished()),this,SLOT(getIndexesReply()));

		return;
	}

	QString data = reply->readAll();

	QJson::Parser parser;
	QVariant json = parser.parse(data.toAscii());

	QVariantList categories = json.toMap()["subsonic-response"].toMap()["indexes"].toMap()["index"].toList();

	QList<IndexFolder*> artists;

	foreach(QVariant artistGroup, categories)
	{
		QVariantList artistsInGroup = artistGroup.toMap()["artist"].toList();

		foreach(QVariant artist, artistsInGroup)
		{
			IndexFolder *artistFolder = new IndexFolder(this);
			QJson::QObjectHelper::qvariant2qobject(artist.toMap(), artistFolder);
			artists.append(artistFolder);
		}
	}

	qDebug()<<" number of artists: "<<artists.count();

	artistsReceived(artists);

	reply->deleteLater();
}

void Subsonic::downloadReply()
{
	qDebug()<<__FUNCTION__;
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QByteArray data = reply->readAll();

	QFile file(mCurrentDownloadFilename);
	if(!file.open(QIODevice:: QIODevice::WriteOnly | QIODevice::Append))
	{
		qDebug()<<"failed to open target file for downloading: "<<file.errorString();
	}

	qDebug()<<"writing "<<data.count()<<" bytes";

	file.write(data);
	file.flush();
	file.close();
}

void Subsonic::download(MusicObject *song, QString filePath)
{   
	ArgMap args;
	args["id"] = song->id();
	//QString url = urlBuilder("download", args);
	QString url = urlBuilder("stream", args);

	qDebug()<<"downloading: "<<filePath;

	QNetworkRequest request;
	request.setUrl(QUrl(url));

	QNetworkReply *reply = 0;
	reply = networkAccessManager->get(request);
	mCurrentDownloadFilename = filePath;

	QFile file(filePath + "_incomplete");
	file.open(QIODevice::Truncate);
	file.write("incomplete");
	file.close();

	connect(reply,SIGNAL(finished()),this,SLOT(downloadFinished()));
	connect(reply,SIGNAL(readyRead()),this,SLOT(downloadReply()));
	connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));

	qDebug()<<__FUNCTION__<<" finished";
}

void Subsonic::getRandomSongs(int num, QString genre, QString fromYear, QString toYear, QString musicFolderId)
{
	ArgMap args;
	args["size"] = QString::number(num);
	if(!genre.isEmpty())
		args["genre"] = genre;
	if(!fromYear.isEmpty())
		args["fromYear"] = fromYear;
	if(!toYear.isEmpty())
		args["toYear"] = toYear;
	if(!musicFolderId.isEmpty())
		args["musicFolderId"] = musicFolderId;

	QString url = urlBuilder("getRandomSongs",args);

	QNetworkRequest request;
	request.setUrl(QUrl(url));

	QNetworkReply *reply = 0;
	reply = networkAccessManager->get(request);

	connect(reply,SIGNAL(finished()),this,SLOT(getMusicDirectoryReply()));
}

void Subsonic::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	bufferProgress = ( (double)bytesReceived / (double)bytesTotal ) * 100;

	qDebug()<<"downloaded: "<<bytesReceived<<" of "<<bytesTotal<< " "<<bufferProgress<<"%";

	downloadBufferChanged(bufferProgress);
}

void Subsonic::downloadFinished()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

	qDebug()<<"is redirect? "<<possibleRedirectUrl;

	if(!possibleRedirectUrl.toUrl().isEmpty())
	{
		qDebug()<<"we got redirected to: "<<possibleRedirectUrl.toUrl().toString();
		reply->deleteLater();
		reply = networkAccessManager->get(QNetworkRequest(possibleRedirectUrl.toUrl()));

		connect(reply,SIGNAL(finished()),this,SLOT(downloadFinished()));
		connect(reply,SIGNAL(readyRead()),this,SLOT(downloadReply()));
		connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));

		return;
	}

//	QDir::home().rename(mCurrentDownloadFilename, mFinalDownloadFilename);

	reply->deleteLater();
}
