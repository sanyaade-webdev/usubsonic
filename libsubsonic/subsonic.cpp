#include "subsonic.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkProxyFactory>

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
	:mServer(server), QObject(parent)
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

void Subsonic::getMusicDirectory(ArtistFolder *folder)
{
	ArgMap args;
	args["id"] = folder->id();
	QString url = urlBuilder("getMusicDirectory", args);

	QNetworkRequest request;
	request.setUrl(QUrl(url));

	QNetworkReply *reply = 0;
	reply = networkAccessManager->get(request);

	connect(reply,SIGNAL(finished()),this,SLOT(getMusicFoldersReply()));
}

void Subsonic::getMusicFoldersReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QString data = reply->readAll();

	qDebug()<<"reply: "<<data;

	QJson::Parser parser;
	QVariant json = parser.parse(data.toAscii());

	QVariant folders = json.toMap()["subsonic-response"].toMap()["musicFolders"].toMap()["musicFolder"];

	qDebug()<<"folders: "<<folders;
}

void Subsonic::getMusicDirectoryReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QString data = reply->readAll();

	qDebug()<<"reply: "<<data;

	QJson::Parser parser;
	QVariant json = parser.parse(data.toAscii());

	QVariantList songsList = json.toMap()["subsonic-response"].toMap()["directory"].toMap()["child"].toList();

	QList<SongObject*> songs;

	foreach(QVariant song, songsList)
	{
		SongObject* songObject = new SongObject(this);

		QJson::QObjectHelper::qvariant2qobject(song.toMap(),songObject);
		songs<<songObject;
	}
}

void Subsonic::getIndexesReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

	if(!reply) return;

	QString data = reply->readAll();

	QJson::Parser parser;
	QVariant json = parser.parse(data.toAscii());

	QVariantList categories = json.toMap()["subsonic-response"].toMap()["indexes"].toMap()["index"].toList();

	QList<ArtistFolder*> artists;

	foreach(QVariant artistGroup, categories)
	{
		QVariantList artistsInGroup = artistGroup.toMap()["artist"].toList();

		foreach(QVariant artist, artistsInGroup)
		{
			ArtistFolder *artistFolder = new ArtistFolder(this);
			QJson::QObjectHelper::qvariant2qobject(artist.toMap(), artistFolder);
			artists.append(artistFolder);
		}
	}

	artistsReceived(artists);
}
